#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "JinGameModeBase.generated.h"

class AJinPlayerController;

UCLASS()
class NUMBASEBALL_API AJinGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void OnPostLogin(AController* NewPlayer) override;
	
	virtual void BeginPlay() override;
	
	void PrintChatMessage(AJinPlayerController* ChattingPlayerController, const FString& ChatMessage);
	
	FString GenerateRandomNum();
	
	bool IsGuessNum(const FString& Num);
	
	FString JudgeResult(const FString& RandomNum, const FString& InputNum);
	
	void IncreaseGuessCount(AJinPlayerController* JinPlayerController);
	
	void ResetGame();
	
	void JudgeGame(AJinPlayerController* JinPlayerController, int StrikeCount);
	
	void SwitchTurn();
	
	bool CanPlayerChat(AJinPlayerController* PC);
	
	void SetFirstPlayer();
	
	FTimerHandle GameResetTimerHandle;

protected:
	FString RandomNumber;
	
	TArray<TObjectPtr<AJinPlayerController>> AllPlayerControllers;
};
