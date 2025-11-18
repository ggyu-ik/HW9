#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "JinPlayerState.generated.h"

UCLASS()
class NUMBASEBALL_API AJinPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Replicated)
	FString PlayerName;
	
	UPROPERTY(Replicated)
	int32 CurrentGuessCount;
	
	UPROPERTY(Replicated)
	int32 MaxGuessCount;
	
	FString GetPlayerInfo();
	
	AJinPlayerState();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
