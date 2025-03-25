#include "ChatController.h"
#include "ChatModeBase.h"
#include "ChatWidget.h"
#include "LoginWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AChatController::AChatController()
{
	// Activate replication for this controller
	bReplicates = true;

	// Login widget
	ConstructorHelpers::FClassFinder<ULoginWidget> LoginWidgetClassFinder(TEXT("/Game/Widgets/WBP_LoginWidget"));
	LoginWidgetClass = LoginWidgetClassFinder.Succeeded() ? LoginWidgetClassFinder.Class : nullptr;
	LoginWidget = nullptr;

	// Chat widget
	ConstructorHelpers::FClassFinder<UChatWidget> ChatWidgetClassFinder(TEXT("/Game/Widgets/WBP_ChatWidget"));
	ChatWidgetClass = ChatWidgetClassFinder.Succeeded() ? ChatWidgetClassFinder.Class : nullptr;
	ChatWidget = nullptr;
}

// Set Username property to replicate over the lifetime of this controller
void AChatController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AChatController, Username);
}

void AChatController::BeginPlay()
{
	Super::BeginPlay();
	
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;

	CreateLoginWidget();

	// If Server, set editable text box as "Host"
	if (UKismetSystemLibrary::IsServer(this))
	{
		ClientSetUsernameTextBox("Host");
	}
	// If Client, do the same except as "Guest"
	else
	{
		ClientSetUsernameTextBox("Guest");
	}
}

/********************* Login : checking and setting Username *********************/
// Create login widget
void AChatController::CreateLoginWidget()
{
	if (LoginWidgetClass && LoginWidget == nullptr && IsLocalController())
	{
		LoginWidget = CreateWidget<ULoginWidget>(this, LoginWidgetClass, TEXT("LoginWidget"));
		if (LoginWidget)
		{
			LoginWidget->AddToViewport();

			// Bind UsernameConfirmed to OnClicked delegate of Confirm UButton
			LoginWidget->ConfirmButton->OnClicked.AddDynamic(this, &AChatController::UsernameConfirmed);
		}
	}
}

// Client RPC - set the text in the login widget
void AChatController::ClientSetUsernameTextBox_Implementation(const FString& InUsername)
{
	if (LoginWidget)
	{
		LoginWidget->SetUsernameText(InUsername);
	}
}

// Call Server RPC to check the username when 'Confirm' button is clicked
void AChatController::UsernameConfirmed()
{
	if (LoginWidget)
	{
		ServerCheckUsername(LoginWidget->Username);
	}
}

// Check if the username already exists (in GameMode)
void AChatController::ServerCheckUsername_Implementation(const FString& InUsername)
{
	if (AChatModeBase* ChatModeBase = Cast<AChatModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		// If the username already exists, call Client RPC to display a "fail" message on the widget
		if (ChatModeBase->UserColorMap.Find(InUsername) != nullptr)
		{
			ClientNotifyUsernameFailed();
			return;
		}
	}
	// Set server-side username
	ServerSetUsername(InUsername);
}

// Display "Username already taken" message on the widget
void AChatController::ClientNotifyUsernameFailed_Implementation()
{
	if (LoginWidget)
	{
		LoginWidget->DisplayUsernameTaken();
	}
}

// Server RPC - set Username and AssignedColor
void AChatController::ServerSetUsername_Implementation(const FString& InUsername)
{
	// Add username to TMap in GameMode
	if (AChatModeBase* ChatModeBase = Cast<AChatModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		ChatModeBase->ServerAddUser(InUsername, this);
	}

	// Update Username variable (replicated)
	Username = InUsername;

	// For the Listen Server Host Client (whose replication doesn't automatically incur OnRep function)
	if (IsLocalController())
	{
		OnRep_UsernameUpdated();
	}
}

// Remove login widget upon successfully replicating the username (= login success)
void AChatController::OnRep_UsernameUpdated()
{
	/*TODO: change this to a pop-up message*/
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, Username + TEXT(" joined the chat."));
	RemoveLoginWidget();
}

// Remove login widget and create chat widget
void AChatController::RemoveLoginWidget()
{
	if (LoginWidget)
	{
		LoginWidget->RemoveFromParent();
		LoginWidget = nullptr;
		EnterLobbyWidget(); // Implemented in Blueprint
	}
}

/********************* Chat : sending / relaying / displaying messages *********************/
// Create chat widget
void AChatController::CreateChatWidget()
{
	if (ChatWidgetClass && ChatWidget == nullptr && IsLocalController())
	{
		ChatWidget = CreateWidget<UChatWidget>(this, ChatWidgetClass, TEXT("ChatWidget"));
		if (ChatWidget)
		{
			ChatWidget->AddToViewport();

			// Bind ServerSendMessage to OnTextCommitted delegate of TextBox
			ChatWidget->MessageTextBox->OnTextCommitted.AddDynamic(this, &AChatController::ServerSendMessage);
		}
	}
}

// Call Server RPC on ChatModeBase to format and relay the message to all player controllers
void AChatController::ServerSendMessage_Implementation(const FText& Text, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::OnEnter)
	{
		if (AChatModeBase* ChatModeBase = Cast<AChatModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			ChatModeBase->ServerRelayMessage(Username, Text);
		}
	}
}

// Client RPC that prints the message relayed from ChatModeBase (server)
void AChatController::ClientPrintMessage_Implementation(const FText& Text, const FColor& Color)
{
	if (HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, Color, Text.ToString());
	}
}

// Remove chat widget
void AChatController::RemoveChatWidget()
{
	if (ChatWidget)
	{
		ChatWidget->RemoveFromParent();
		ChatWidget = nullptr;
	}
}

/********************* DEBUG : PRINTING ON-SCREEN MSGS *********************/
// Print Username on Server & queue Client RPC to print Username on Client
void AChatController::ServerPrintUsername_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Magenta, TEXT("Server Username: ") + Username);

	GetWorldTimerManager().SetTimer(
		LoginTimer,
		this,
		&AChatController::ClientPrintUsername,
		0.5f,
		false
	);
}

// Print Username on Client
void AChatController::ClientPrintUsername_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Magenta, TEXT("Client Username: ") + Username);
}