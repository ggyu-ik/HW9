#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "JinGameStateBase.generated.h"

class AJinPlayerState;

UCLASS()
class NUMBASEBALL_API AJinGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InName);
	
	UPROPERTY(Replicated)
	AJinPlayerState* CurrentPlayer = nullptr;
	
	float TurnTimeLimit;
	
	UPROPERTY(Replicated)
	float RemainingTime; // 턴 남은 시간
	
	FTimerHandle TurnTimeLimitTimerHandle;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
