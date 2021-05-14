// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Functions
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();
	void CheckPressurePlate();

private:
	//Variables
	bool OpenDoorSound = false;
	bool CloseDoorSound = true;

	float InitialYaw;
	float CurrentYaw;

	UPROPERTY(EditAnywhere);
	float OpenAngle = 90.f;

	UPROPERTY(EditAnywhere);
	ATriggerVolume* PressurePlate = nullptr;

	AActor* MyCharactor = nullptr;

	float DoorLastOpen = 0.f;

	UPROPERTY(EditAnywhere);
	float DoorOpenTime = 0.3f;

	UPROPERTY(EditAnywhere);
	float DoorOpenSpeed = 0.8f;

	UPROPERTY(EditAnywhere);
	float DoorCloseSpeed = 0.2f;

	UPROPERTY(EditAnywhere);
	float MassNeed = 60.f;

	UPROPERTY();
	UAudioComponent* AudioComponent = nullptr;
};
