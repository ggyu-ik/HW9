#include "System/JinPlayerState.h"
#include "Net/UnrealNetwork.h"

FString AJinPlayerState::GetPlayerInfo()
{
	FString PlayerGuessInfo = PlayerName + TEXT("(") + FString::FromInt(CurrentGuessCount) + TEXT("/") + FString::FromInt(MaxGuessCount) + TEXT(")");
	return PlayerGuessInfo;
}

AJinPlayerState::AJinPlayerState()
	: PlayerName(""), CurrentGuessCount(0), MaxGuessCount(3)
{
}

void AJinPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerName);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);
}
