#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JinPlayerController.generated.h"

class UJinInputWidget;
class UUserWidget;

UCLASS()
class NUMBASEBALL_API AJinPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	void SetChatMessage(const FString& InputMessage);
	
	void PrintChatMessage(const FString& InputMessage);
	
	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessage(const FString& InputMessage);
	
	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessage(const FString& InputMessage);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(Client, Reliable)
	void ClientRPCShowNotificationWidget(float ShowSeconds);
	
	void HideNotificationWidget();
	
	bool IsMyTurn() const;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
	
	FTimerHandle ShowNotificationTimerHandle;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UJinInputWidget> InputWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UJinInputWidget> InputWidgetInstance;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;
};
