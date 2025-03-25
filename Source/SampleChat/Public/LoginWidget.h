#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

class UVerticalBox;

UCLASS()
class SAMPLECHAT_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (BindWidget))
	class UEditableTextBox* UsernameTextBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (BindWidget))
	class UVerticalBox* UsernameVBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (BindWidget))
	class UButton* ConfirmButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (BindWidget))
	class UButton* BackButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (BindWidget))
	UVerticalBox* MessageBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (BindWidget))
	class UTextBlock* MessageText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	FString Username;
	
	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SetUsernameText(const FString& InUsername);
	UFUNCTION(BlueprintCallable)
	void CommitUsernameText(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION(BlueprintCallable)
	void DisplayMessageBox();
	UFUNCTION(BlueprintCallable)
	void DisplayEmptyUsername();
	UFUNCTION(BlueprintCallable)
	void DisplayUsernameTaken();
	UFUNCTION(BlueprintCallable)
	void HideMessageBox();
};
