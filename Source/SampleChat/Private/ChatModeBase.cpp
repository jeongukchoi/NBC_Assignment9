#include "ChatModeBase.h"
#include "ChatController.h"

void AChatModeBase::ServerAddUser_Implementation(const FString& Username, APlayerController* PlayerController)
{
	UserColorMap.Add(Username, FColor::MakeRandomColor());
	UserControllerMap.Add(Username, PlayerController);
	RemainingGuesses.Add(Username, 3);
	Usernames.Add(Username);
}

void AChatModeBase::ServerRelayMessage_Implementation(const FString& Username, const FText& Message)
{
	if (!Message.ToString().IsEmpty() && Message.ToString().StartsWith("/"))
	{
		ProcessPlayerGuess(Username, Message);
		return;
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; Iterator++)
	{
		if (AChatController* ChatController = Cast<AChatController>(*Iterator))
		{
			FColor Color = FColor::White;
			if (FColor* ColorPtr = UserColorMap.Find(Username))
			{
				Color = *ColorPtr;
			}
			ChatController->ClientPrintMessage(FText::FromString(Username + TEXT(": ") + Message.ToString()), Color);
		}
	}
}