// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "Projectile.h"
#include "NiagaraFunctionLibrary.h"
#include "BasePawn.generated.h"

UCLASS()
class BATTLEBLASTER_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* ProjectileSpawnPoint;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* DestructionParticles;
	UPROPERTY(EditAnywhere)
	USoundBase* DestructionSound;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> DestructionCameraShakeClass;

	void RotateTurret(FVector LookAtTarget);
	void Fire();
	void HandleDestruction();
};
