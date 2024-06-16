#include "Libraries/Logger.h"

#include "Engine/Engine.h"

DEFINE_LOG_CATEGORY(DiabloLikeARPG);

void FLogger::Log(const FString& Message, const ELogLevel CustomLogLevel, const bool bShouldPrintToScreen, const float TimeToDisplay)
{
	FColor Color;
	switch (CustomLogLevel)
	{
	case ELogLevel::Info:
		UE_LOG(DiabloLikeARPG, Display, TEXT("%s"), *Message);
		Color = FColor::White;
		break;
	case ELogLevel::Warning:
		UE_LOG(DiabloLikeARPG, Warning, TEXT("%s"), *Message);
		Color = FColor::Yellow;
		break;
	case ELogLevel::Error:
		UE_LOG(DiabloLikeARPG, Error, TEXT("%s"), *Message);
		Color = FColor::Red;
		break;
	}

	if(bShouldPrintToScreen)
	{
		LogToScreen(Message, Color, TimeToDisplay);
	}
}

void FLogger::LogToScreen(const FString& Message, const FColor& Color, const float TimeToDisplay)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Color, Message);
}
