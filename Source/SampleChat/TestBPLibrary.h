#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TestBPLibrary.generated.h"

UCLASS()
class SAMPLECHAT_API UTestBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Test")
	static FString GetTestString();
};
