#include "BaseballGameLibrary.h"

TArray<uint8> UBaseballGameLibrary::GetRandomDigits()
{
	TSet<uint8> RandomDigitsSet;
	while (RandomDigitsSet.Num() < 3)
	{
		uint8 Digit = FMath::RandRange(1, 9);
		RandomDigitsSet.Add(Digit);
	}
	return TArray<uint8>(RandomDigitsSet.Array());
}

TArray<uint8> UBaseballGameLibrary::GetResults(const FText& InText, const TArray<uint8>& TargetDigits)
{
	// Array to save Strikes, Balls, and Out
	TArray<uint8> Results;
	Results.SetNumZeroed(static_cast<uint8>(EResult::MAX));
	
	// Convert InText (e.g. /123) to TArray<uint8> (e.g. [1, 2, 3])
	FString InString = InText.ToString();
	InString.RemoveAt(0);
	
	TArray<uint8> InputDigits;
	for (char DigitChar : InString)
	{
		uint8 Digit = DigitChar - '0';
		InputDigits.Add(Digit);
	}

	// OUT if the number of digits don't match
	if (InputDigits.Num() != TargetDigits.Num())
	{
		Results[static_cast<uint8>(EResult::Out)] = 1;
		return Results;
	}
	// OUT if there are repeated digits
	bool bHasRepeatedDigits = false;
	TSet<uint8> DigitsSet;
	for (uint8 Digit : InputDigits)
	{
		if (DigitsSet.Contains(Digit))
		{
			bHasRepeatedDigits = true;
			break;
		}
		DigitsSet.Add(Digit);
	}
	if (bHasRepeatedDigits)
	{
		Results[static_cast<uint8>(EResult::Out)] = 1;
		return Results;
	}
	
	// Calculate Strikes and Balls
	for (uint8 i = 0; i < InputDigits.Num(); i++)
	{
		if (InputDigits[i] == TargetDigits[i])
		{
			Results[static_cast<uint8>(EResult::Strike)]++;
			continue;
		}

		if (TargetDigits.Contains(InputDigits[i]))
		{
			Results[static_cast<uint8>(EResult::Ball)]++;
		}
	}

	return Results;
}
