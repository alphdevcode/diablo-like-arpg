#pragma once

DECLARE_LOG_CATEGORY_EXTERN(GameLogger, Display, All);

#define LOG_INFO(Message, ...) FLogger::Log(FString::Printf(Message, ##__VA_ARGS__), ELogLevel::Info)
#define LOG_WARNING(Message, ...) FLogger::Log(FString::Printf(Message, ##__VA_ARGS__), ELogLevel::Warning)
#define LOG_ERROR(Message, ...) FLogger::Log(FString::Printf(Message, ##__VA_ARGS__), ELogLevel::Error)

enum class ELogLevel
{
	Info,
	Warning,
	Error
};


class LOGGINGSYSTEM_API FLogger
{
public:
	static void Log(const FString& Message, const ELogLevel CustomLogLevel, const bool bShouldPrintToScreen = true,
		const float TimeToDisplay = 2.f);

private:
	static void LogToScreen(const FString& Message, const FColor& Color, float TimeToDisplay);
};
