#include "System/JinGameModeBase.h"

#include "JinGameStateBase.h"
#include "Controller/JinPlayerController.h"
#include "EngineUtils.h"
#include "Components/SlateWrapperTypes.h"
#include "System/JinPlayerState.h"

AJinGameModeBase::AJinGameModeBase()
	: TurnTimeLimit(30.0f)
{
}

void AJinGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	AJinPlayerController* JinPlayerController = Cast<AJinPlayerController>(NewPlayer);
	
	if (IsValid(JinPlayerController))
	{
		JinPlayerController->NotificationText = FText::FromString(TEXT("서버에 연결됐습니다."));
		JinPlayerController->ClientRPCShowNotificationWidget(2.0f);
		
		AllPlayerControllers.Add(JinPlayerController);
		
		AJinPlayerState* JinPS = JinPlayerController->GetPlayerState<AJinPlayerState>();
		if (IsValid(JinPS))
		{
			JinPS->PlayerName = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}
		
		AJinGameStateBase* JinGS = GetGameState<AJinGameStateBase>();
		if (IsValid(JinGS))
		{
			JinGS->MulticastRPCBroadcastLoginMessage(JinPS->PlayerName);
		}
	}
	
	if (AllPlayerControllers.Num() == 2) // 2명이상이 되면 첫 플레이어 정함
	{
		GetWorldTimerManager().SetTimer(
			SwitchTurnDelayTimerHandle,
			this,
			&AJinGameModeBase::SwitchTurn,
			0.1f,
			false);
	}
}

void AJinGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	RandomNumber = GenerateRandomNum();	
}

void AJinGameModeBase::PrintChatMessage(AJinPlayerController* ChattingPlayerController, const FString& ChatMessage)
{
	FString GuessNum = ChatMessage.Right(3);
	
	AJinPlayerState* JinPS = ChattingPlayerController->GetPlayerState<AJinPlayerState>();
	
	if (IsValid(JinPS))
	{
		if (IsGuessNum(GuessNum))
		{
			IncreaseGuessCount(ChattingPlayerController);

			FString Judge = JudgeResult(RandomNumber, GuessNum);
			int32 StrikeCount = FCString::Atoi(*Judge.Left(1));
			
			FString GuessMessage = JinPS->GetPlayerInfo()+ TEXT(": ") + ChatMessage + TEXT(" -> ") + Judge;

			for (TActorIterator<AJinPlayerController> It(GetWorld()); It; ++It)
			{
				AJinPlayerController* JinPlayerController = *It;
				if (IsValid(JinPlayerController))
				{
					JinPlayerController->ClientRPCPrintChatMessage(GuessMessage);
					
					JudgeGame(ChattingPlayerController, StrikeCount);
				}
			}
			if (JinPS->CurrentGuessCount >= 3 && StrikeCount != 3)
			{
				SwitchTurn();
			}
		}
		else
		{
			FString NoneGuessMessage = JinPS->GetPlayerInfo()+ TEXT(": ") + ChatMessage;
			for (TActorIterator<AJinPlayerController> It(GetWorld()); It; ++It)
			{
				AJinPlayerController* JinPlayerController = *It;
				if (IsValid(JinPlayerController))
				{
					JinPlayerController->ClientRPCPrintChatMessage(NoneGuessMessage);
				}
			}
		}
	}
}

FString AJinGameModeBase::GenerateRandomNum()
{
	TSet<int32> Numbers; // Set으로 중복방지
	
	while (Numbers.Num() < 3) // 3개 값 뽑을 때 까지 루프
	{
		Numbers.Add(FMath::RandRange(1, 9)); // 1~9 값 랜덤
	}
	
	FString Result;
	
	for (int32 Num : Numbers)
	{
		Result.Append(FString::FromInt(Num));
	}
	
	return Result;
}

bool AJinGameModeBase::IsGuessNum(const FString& Num)
{
	bool bCanPlay = false;
	
	do 
	{
		if (Num.Len() != 3)
		{
			break;
		}
		
		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		
		for (TCHAR C : Num)
		{
			if (!FChar::IsDigit(C) || C == '0')
			{
				bIsUnique = false;
				break;
			}
			UniqueDigits.Add(C);
		}
		
		if (!bIsUnique || UniqueDigits.Num() != 3)
		{
			break;
		}
		
		bCanPlay = true;

	} while (false);

	return bCanPlay;
}

FString AJinGameModeBase::JudgeResult(const FString& RandomNum, const FString& InputNum)
{
	int32 StrikeCount = 0;
	int32 BallCount = 0;
	
	for (int32 i = 0; i < 3; ++i)
	{
		if (RandomNum[i] == InputNum[i])
		{
			++StrikeCount;
		}
		else
		{
			FString PlayerGuessNum = FString::Printf(TEXT("%c"), InputNum[i]);
			if (RandomNum.Contains(PlayerGuessNum))
			{
				++BallCount;
			}
		}
	}
	
	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("아웃");
	}
	
	return FString::Printf(TEXT("%d 스트라이크, %d 볼"), StrikeCount, BallCount);
}

void AJinGameModeBase::IncreaseGuessCount(AJinPlayerController* ChattingPlayerController)
{
	AJinPlayerState* JinPS = ChattingPlayerController->GetPlayerState<AJinPlayerState>();
	if (IsValid(JinPS))
	{
		JinPS->CurrentGuessCount++;
	}
}

void AJinGameModeBase::ResetGame()
{
	RandomNumber = GenerateRandomNum();
	
	AJinGameStateBase* JinGS = GetGameState<AJinGameStateBase>();

	JinGS->RemainingTime = 30.0f;
	
	if(IsValid(JinGS))
	{
		for (const auto& PC : AllPlayerControllers)
		{
			AJinPlayerState* JinPS = PC->GetPlayerState<AJinPlayerState>();
			
			if (IsValid(JinPS))
			{
				JinPS->CurrentGuessCount = 0;
			}
			
			PC->NotificationText = FText::FromString(FString::Printf(TEXT("%s 의 차례입니다."), *JinGS->CurrentPlayer->PlayerName));
			PC->ClientRPCShowNotificationWidget(TurnTimeLimit);
		}
	}

	GetWorldTimerManager().SetTimer(
		TurnTimeLimitTimerHandle,
		this,
		&AJinGameModeBase::SwitchTurn,
		TurnTimeLimit,
		false);

	GetWorldTimerManager().SetTimer(
			TurnTimerHandle,
			this,
			&AJinGameModeBase::TurnTimer,
			1.0f,
			true);
}

void AJinGameModeBase::JudgeGame(AJinPlayerController* ChattingPlayerController, int StrikeCount)
{
	if (StrikeCount == 3)
	{
		AJinPlayerState* JinPS = ChattingPlayerController->GetPlayerState<AJinPlayerState>();
		for (const auto& PC : AllPlayerControllers)
		{
			FString CombineMessage = JinPS->PlayerName + TEXT("님이 이겼습니다. 5초 후 새로운 게임이 시작됩니다.");
			PC->NotificationText = FText::FromString(CombineMessage);
			PC->ClientRPCShowNotificationWidget(5.0f);
		}
		
		GetWorldTimerManager().SetTimer(
		GameResetTimerHandle,
		this,
		&AJinGameModeBase::ResetGame,
		5.0f,
		false);
	}
}

void AJinGameModeBase::SwitchTurn()
{
	GetWorldTimerManager().ClearTimer(TurnTimeLimitTimerHandle);
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	
	AJinGameStateBase* JinGS = GetGameState<AJinGameStateBase>();
	
	if (!IsValid(JinGS) || AllPlayerControllers.Num() == 0)
	{
		return;
	}

	JinGS->RemainingTime = 30.0f;
	
	if (JinGS->CurrentPlayer == nullptr)
	{
		AJinPlayerState* FirstPlayerPS = AllPlayerControllers[0]->GetPlayerState<AJinPlayerState>();

		if (IsValid(FirstPlayerPS))
		{
			JinGS->CurrentPlayer = FirstPlayerPS;
			for (const auto& PC : AllPlayerControllers)
			{
				PC->NotificationText = FText::FromString(FString::Printf(TEXT("%s 의 차례입니다."), *FirstPlayerPS->PlayerName));
				PC->ClientRPCShowNotificationWidget(TurnTimeLimit);
			}
			
			GetWorldTimerManager().SetTimer(
			TurnTimeLimitTimerHandle,
			this,
				&AJinGameModeBase::SwitchTurn,
			TurnTimeLimit,
			false);
			
			GetWorldTimerManager().SetTimer(
			TurnTimerHandle,
			this,
			&AJinGameModeBase::TurnTimer,
			1.0f,
			true);
		}
		return;
	}
	
	if (AllPlayerControllers.Num() == 2) // 2인 기준
	{
		AJinPlayerState* Player1 = AllPlayerControllers[0]->GetPlayerState<AJinPlayerState>();
		AJinPlayerState* Player2 = AllPlayerControllers[1]->GetPlayerState<AJinPlayerState>();
		
		if (JinGS->CurrentPlayer == Player1)
		{
			Player2->CurrentGuessCount = 0;
			JinGS->CurrentPlayer = Player2;
			
			for (const auto& PC:AllPlayerControllers)
			{
				PC->NotificationText = FText::FromString(FString::Printf(TEXT("%s 의 차례입니다."), *Player2->PlayerName));
				PC->ClientRPCShowNotificationWidget(TurnTimeLimit);
			}
		}
		else
		{
			Player1->CurrentGuessCount = 0;
			JinGS->CurrentPlayer = Player1;
			
			for (const auto& PC:AllPlayerControllers)
			{
				PC->NotificationText = FText::FromString(FString::Printf(TEXT("%s 의 차례입니다."), *Player1->PlayerName));
				PC->ClientRPCShowNotificationWidget(TurnTimeLimit);
			}
		}
		
		GetWorldTimerManager().SetTimer(
			TurnTimeLimitTimerHandle,
			this,
			&AJinGameModeBase::SwitchTurn,
			TurnTimeLimit,
			false);

		GetWorldTimerManager().SetTimer(
			TurnTimerHandle,
			this,
			&AJinGameModeBase::TurnTimer,
			1.0f,
			true);
	}
}

bool AJinGameModeBase::CanPlayerChat(AJinPlayerController* PC)
{
	AJinGameStateBase* JinGS = GetGameState<AJinGameStateBase>();
	AJinPlayerState* PS = PC->GetPlayerState<AJinPlayerState>();
	
	if (IsValid(JinGS) && IsValid(PS))
	{
		if (JinGS->CurrentPlayer == nullptr)
		{
			return true;
		}
		
		return JinGS->CurrentPlayer == PS;
	}
	
	return true;
}

void AJinGameModeBase::TurnTimer()
{
	AJinGameStateBase* JinGS = GetGameState<AJinGameStateBase>();
	if (IsValid(JinGS))
	{
		JinGS->RemainingTime--;
	}
}