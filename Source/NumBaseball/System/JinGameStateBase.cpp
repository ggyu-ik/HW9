#include "System/JinGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Controller/JinPlayerController.h"
#include "Net/UnrealNetwork.h"

void AJinGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InName)
{
	if (!HasAuthority()) // 클라이언트인지 확인
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC))
		{
			AJinPlayerController* JinPC = Cast<AJinPlayerController>(PC);
			if (IsValid(JinPC))
			{
				FString Notification = InName + TEXT(" 님이 로그인 했습니다.");
				JinPC->PrintChatMessage(Notification);
			}
		}
	}
}

void AJinGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AJinGameStateBase, CurrentPlayer);
}
