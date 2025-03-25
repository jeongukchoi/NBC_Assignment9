#include "ChatWidget.h"
#include "Components/EditableTextBox.h"

void UChatWidget::NativeConstruct()
{
	MessageTextBox->OnTextCommitted.AddDynamic(this, &UChatWidget::EmptyTextBox);
}

void UChatWidget::EmptyTextBox(const FText& Text, ETextCommit::Type CommitType)
{
	MessageTextBox->SetText(FText::GetEmpty());
}


