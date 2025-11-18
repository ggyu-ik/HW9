#include "Controller/JinPlayerController.h"

#include "Kismet/KismetSystemLibrary.h"
#include "UI/JinInputWidget.h"
#include "EngineUtils.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "System/JinGameModeBase.h"
#include "System/JinPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "System/JinGameStateBase.h"

void AJinPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsLocalController())
	{
		return;
	}
	
	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);
	SetShowMouseCursor(true); // 마우스 커서 보이게
	
	if (IsValid(InputWidgetClass))
	{
		InputWidgetInstance = CreateWidget<UJinInputWidget>(this, InputWidgetClass);
		
		if (IsValid(InputWidgetInstance))
		{
			InputWidgetInstance->AddToViewport();
		}
	}
	
	if (IsValid(NotificationTextWidgetClass))
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance))
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
}

void AJinPlayerController::SetChatMessage(const FString& InputMessage)
{
	if (!IsLocalController())
	{
		return;
	}
	
	if (!IsMyTurn())
	{
		UKismetSystemLibrary::PrintString(
			this,
			TEXT("지금은 상대 턴입니다."),
			true,
			false,
			FLinearColor::Yellow,
			2.0f);

		return;
	}
	
	ServerRPCPrintChatMessage(InputMessage);
}

void AJinPlayerController::PrintChatMessage(const FString& InputMessage)
{
	UKismetSystemLibrary::PrintString(this, InputMessage, true, true, FLinearColor::Red, 5.0f);
}

void AJinPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, NotificationText);
}

void AJinPlayerController::ClientRPCShowNotificationWidget_Implementation(float ShowSeconds)
{
	if (!IsLocalController())
	{
		return;
	}
	
	if (IsValid(NotificationTextWidgetInstance))
	{
		UTextBlock* NotificationTextBlock = Cast<UTextBlock>(NotificationTextWidgetInstance->GetWidgetFromName(TEXT("NotificationText")));
		if (IsValid(NotificationTextBlock))
		{
			NotificationTextBlock->SetVisibility(ESlateVisibility::Visible);
		}
	}
	GetWorldTimerManager().ClearTimer(ShowNotificationTimerHandle);
	
	GetWorldTimerManager().SetTimer(
		ShowNotificationTimerHandle,
		this,
		&AJinPlayerController::HideNotificationWidget,
		ShowSeconds,
		false);
}

void AJinPlayerController::HideNotificationWidget()
{	
	if (!IsLocalController())
	{
		return;
	}
	
	if (IsValid(NotificationTextWidgetInstance))
	{
		UTextBlock* NotificationTextBlock = Cast<UTextBlock>(NotificationTextWidgetInstance->GetWidgetFromName(TEXT("NotificationText")));
		if (IsValid(NotificationTextBlock))
		{
			NotificationTextBlock->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

bool AJinPlayerController::IsMyTurn() const
{
	AJinGameStateBase* JinGS = GetWorld()->GetGameState<AJinGameStateBase>();
	AJinPlayerState* MyPS = GetPlayerState<AJinPlayerState>();
	
	if (IsValid(JinGS) && IsValid(MyPS))
	{
		if (JinGS->CurrentPlayer == nullptr)
		{
			return true;
		}
		
		return JinGS->CurrentPlayer == MyPS;
	}
	
	return true;
}

void AJinPlayerController::ClientRPCPrintChatMessage_Implementation(const FString& InputMessage)
{
	PrintChatMessage(InputMessage);
}

void AJinPlayerController::ServerRPCPrintChatMessage_Implementation(const FString& InputMessage)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM))
	{
		AJinGameModeBase* JinGM = Cast<AJinGameModeBase>(GM);
		if (IsValid(JinGM))
		{
			if (!JinGM->CanPlayerChat(this))
			{
				return;
			}
			
			JinGM->PrintChatMessage(this, InputMessage);
		}
	}
}