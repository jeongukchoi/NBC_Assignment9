#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BaseballGameLibrary.generated.h"

UENUM(BlueprintType)
enum class EResult : uint8
{
	Strike,
	Ball,
	Out,
	MAX
};

UCLASS()
class SAMPLECHAT_API UBaseballGameLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "BaseballGame")
	static TArray<uint8> GetRandomDigits();

	UFUNCTION(BlueprintCallable, Category = "BaseballGame")
	static TArray<uint8> GetResults(const FText& InText, const TArray<uint8>& TargetDigits);
};
