// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "BattleBlasterGameInstance.h"
#include "Tower.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);
	TowerCount = Towers.Num();

	UE_LOG(LogTemp, Display, TEXT("Number of Tower is: %d"), TowerCount);
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		Tank = Cast<ATank>(PlayerPawn);
		if (!Tank)
		{
			UE_LOG(LogTemp, Display, TEXT("Gamemode failed to found tank actor"));
		}
		else 
		{
			UE_LOG(LogTemp, Display, TEXT("Gamemode found tank actor"));
			int32 temp = 0;
			while (temp < TowerCount)
			{
				ATower* Tower = Cast<ATower>(Towers[temp]);
				Tower->Tank = Tank;
				UE_LOG(LogTemp, Display, TEXT("%s is setting the tank variable"), *Tower->GetActorNameOrLabel());

				temp++;
			}
		}
	}
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController) 
	{
		ScreenMessageWidget = CreateWidget<UScreenMessage>(PlayerController,ScreenMessageClass);
		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->AddToPlayerScreen();
			ScreenMessageWidget->SetMessageText("Get Ready!");
		}
	}
	

	CountdownSeconds = CountdownDelay;
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ABattleBlasterGameMode::OnCountDownTimerTimeout, 1.0f, true);
}


void ABattleBlasterGameMode::OnCountDownTimerTimeout()
{
	FString CountdownText;
	if (CountdownSeconds > 0)
	{
		ScreenMessageWidget->SetMessageText(FString::FromInt(CountdownSeconds));
	}
	else if(CountdownSeconds == 0)
	{
		CountdownText = "GOOOOO!!!";
		ScreenMessageWidget->SetMessageText(CountdownText);
		Tank->SetPlayerEnabled(true);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Hidden);

	}
	CountdownSeconds--;
}

void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{
	bool IsGameOver = false;
	if (DeadActor == Tank) 
	{
		Tank->HandleDestruction();
		IsGameOver = true;
	}
	else
	{
		ATower* DeadTower = Cast<ATower>(DeadActor);
		if (DeadTower) 
		{
			DeadTower->HandleDestruction();
			TowerCount--;
			if (TowerCount == 0) 
			{
				IsGameOver = true;
				IsVictory = true;

			}
		}
		
	}

	if (IsGameOver)
	{
		FString GameOverString = IsVictory ?  "Victory" : "Defeat";
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Visible);
		ScreenMessageWidget->SetMessageText(GameOverString);
		FTimerHandle TimerHandler;
		GetWorldTimerManager().SetTimer(TimerHandler, this, &ABattleBlasterGameMode::OnGameOverTimerTimeout, GameOverDelay, false);
	}

}

void ABattleBlasterGameMode::OnGameOverTimerTimeout()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UBattleBlasterGameInstance* BattleBlasterGameInstance = Cast<UBattleBlasterGameInstance>(GameInstance);
		if (BattleBlasterGameInstance)
		{
			if (IsVictory)
			{
				BattleBlasterGameInstance->LoadNextLevel();
			}
			else
			{
				BattleBlasterGameInstance->RestartCurrentLevel();
			}
		}
	}

}

