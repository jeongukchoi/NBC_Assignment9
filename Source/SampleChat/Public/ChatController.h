#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChatController.generated.h"

UCLASS()
class SAMPLECHAT_API AChatController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// Login Widget class & instance
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<UUserWidget> LoginWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	class ULoginWidget* LoginWidget;
	
	// Chat Widget class & instance
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<UUserWidget> ChatWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	class UChatWidget* ChatWidget;
	
	// Username (RepNotify)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RepProperties", ReplicatedUsing=OnRep_UsernameUpdated)
	FString Username;

	// Delay TimerHandle for replication
	FTimerHandle LoginTimer;

	AChatController();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Login - Validate / Set Username
	UFUNCTION(BlueprintCallable)
	void CreateLoginWidget();
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ClientSetUsernameTextBox(const FString& InUsername);
	UFUNCTION(BlueprintCallable)
	void UsernameConfirmed();
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerCheckUsername(const FString& InUsername);
	UFUNCTION(BlueprintCallable, Client, Unreliable)
	void ClientNotifyUsernameFailed();
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerSetUsername(const FString& InUsername);
	UFUNCTION(BlueprintCallable)
	void OnRep_UsernameUpdated();
	UFUNCTION(BlueprintCallable)
	void RemoveLoginWidget();

	// Lobby - Set Ready / Start Game - implemented in Blueprint
	UFUNCTION(BlueprintImplementableEvent)
	void EnterLobbyWidget();

	// Chat - Send / receive / print message
	UFUNCTION(BlueprintCallable)
	void CreateChatWidget();
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerSendMessage(const FText& Text, ETextCommit::Type CommitType);
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ClientPrintMessage(const FText& Text, const FColor& Color);
	UFUNCTION(BlueprintCallable)
	void RemoveChatWidget();

	// DEBUG - print server/client username
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerPrintUsername();
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ClientPrintUsername();

private:
	virtual void BeginPlay() override;
};
