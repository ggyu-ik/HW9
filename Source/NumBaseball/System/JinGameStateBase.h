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
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	float RemainingTime;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
