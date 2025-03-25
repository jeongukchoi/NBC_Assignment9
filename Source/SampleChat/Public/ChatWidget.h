#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatWidget.generated.h"

UCLASS()
class SAMPLECHAT_API UChatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widgets", meta=(BindWidget))
	class UEditableTextBox* MessageTextBox;
	
	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void EmptyTextBox(const FText& Text, ETextCommit::Type CommitType);
};
