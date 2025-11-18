#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SlateEnums.h"
#include "JinInputWidget.generated.h"

class UEditableTextBox;

UCLASS()
class NUMBASEBALL_API UJinInputWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
protected:
	UFUNCTION()
	void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> InputTextBox;
};
