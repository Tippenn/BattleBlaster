// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"


void ATower::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle FireTimerHandle;
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (InFireRange()) 
	{
		RotateTurret(Tank->GetActorLocation());
	}
}

void ATower::CheckFireCondition()
{
	if (InFireRange() )
	{
		Fire();
	}
}

bool ATower::InFireRange()
{
	if (!Tank) return false;
	float DistanceToTank = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
	return (DistanceToTank < FireRange) && (Tank->IsAlive);
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();
	UE_LOG(LogTemp, Display, TEXT("Tower Handle Destruction"))

		Destroy();

}
