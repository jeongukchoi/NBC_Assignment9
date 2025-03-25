#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ChatModeBase.generated.h"


UCLASS()
class SAMPLECHAT_API AChatModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerInfo")
	TMap<FString, FColor> UserColorMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerInfo")
	TMap<FString, APlayerController*> UserControllerMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerInfo")
	TMap<FString, int32> RemainingGuesses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerInfo")
	TArray<FString> Usernames;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerAddUser(const FString& UserName, APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerRelayMessage(const FString& Username, const FText& Message);

	UFUNCTION(BlueprintImplementableEvent)
	void ProcessPlayerGuess(const FString& Username, const FText& InText);
};
