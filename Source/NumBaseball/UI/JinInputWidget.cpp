#include "UI/JinInputWidget.h"
#include "Components/EditableTextBox.h"
#include "Controller/JinPlayerController.h"


void UJinInputWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 델리게이트 바인드가 안되어 있으면 바인드
	if (!InputTextBox->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted))
	{
		InputTextBox->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UJinInputWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	// 델리게이트 바인드가 되어 있으면 바인드 제거
	if (InputTextBox->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted))
	{
		InputTextBox->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UJinInputWidget::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter) // 엔터를 치면
	{
		APlayerController* PlayerController = GetOwningPlayer();
		if (IsValid(PlayerController))
		{
			AJinPlayerController* JinPlayerController= Cast<AJinPlayerController>(PlayerController);
			if (IsValid(JinPlayerController))
			{
				JinPlayerController->SetChatMessage(Text.ToString());
				
				InputTextBox->SetText(FText());
			}
		}
	}
}

