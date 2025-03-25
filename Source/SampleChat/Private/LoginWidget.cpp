#include "LoginWidget.h"
#include "ChatController.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"

void ULoginWidget::NativeConstruct()
{
	UsernameTextBox->OnTextCommitted.AddDynamic(this, &ULoginWidget::CommitUsernameText);
	BackButton->OnClicked.AddDynamic(this, &ULoginWidget::HideMessageBox);
}

void ULoginWidget::SetUsernameText(const FString& UserID)
{
	UsernameTextBox->SetText(FText::FromString(UserID));
}

void ULoginWidget::CommitUsernameText(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		Username = Text.ToString();
		UsernameTextBox->SetText(FText::GetEmpty());
		if (Username.IsEmpty())
		{
			DisplayEmptyUsername();
		}
		else
		{
			DisplayMessageBox();
		}
	}
}

void ULoginWidget::DisplayMessageBox()
{
	// Collapse Username Text Box and display Mesage Box to confirm/cancel Username
	UsernameVBox->SetVisibility(ESlateVisibility::Collapsed);
	MessageBox->SetVisibility(ESlateVisibility::Visible);
	ConfirmButton->SetVisibility(ESlateVisibility::Visible);
	BackButton->SetVisibility(ESlateVisibility::Visible);
	
	FString MessageString = TEXT("Proceed with ") + Username + TEXT(" as your username?");
	MessageText->SetText(FText::FromString(MessageString));
}

void ULoginWidget::DisplayEmptyUsername()
{
	MessageText->SetText(FText::FromString(TEXT("Username cannot be empty. Please enter a different username.")));
	ConfirmButton->SetVisibility(ESlateVisibility::Collapsed);
}

void ULoginWidget::DisplayUsernameTaken()
{
	MessageText->SetText(FText::FromString(TEXT("'") + Username + TEXT("' is already taken. Please enter a different username.")));
	Username = "";
	ConfirmButton->SetVisibility(ESlateVisibility::Collapsed);
}

void ULoginWidget::HideMessageBox()
{
	MessageBox->SetVisibility(ESlateVisibility::Collapsed);
	UsernameVBox->SetVisibility(ESlateVisibility::Visible);
}
