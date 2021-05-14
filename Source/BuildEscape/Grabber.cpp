// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

// Get the end location that line traced
FVector UGrabber::GetLineTraceEnd() const
{
	FVector Playerlocation;
	FRotator Playerrotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT Playerlocation, OUT Playerrotation);
	return Playerlocation + Reach * (Playerrotation.Vector());
}


void UGrabber::SetupInputComponent()
{
	// Set InputComponent and Bind the Mouse action.
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Input component found"));
		InputComponent->BindAction("Grab",IE_Pressed,this,&UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot find InputComponent component on %s !!!"), *GetOwner()->GetName());
	}
}


void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot find PhysicsHandle component on %s !!!"), *GetOwner()->GetName());
	}
}


void UGrabber::Grab()
{
	FHitResult HitResult = GetFirstPhysicsBodyInReach();

	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	// Check Actor is not null,then Grab it.
	if (ActorHit)
	{
		if (!PhysicsHandle){return;}
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None,
			GetLineTraceEnd()
		);
	}
}

void UGrabber::Release()
{
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Debug line
	//DrawDebugLine(GetWorld(),GetOwner()->GetActorLocation(),GetLineTraceEnd(),FColor(125,125,125),false,0.f,0,5);

	FHitResult Hit;
	FCollisionQueryParams TraceParams(*GetOwner()->GetName(),false,GetOwner());
	bool result = GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayersWorldPos(),
		GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
		);

	return Hit;
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
}

FVector UGrabber::GetPlayersWorldPos() const
{
	FVector Playerlocation;
	FRotator Playerrotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT Playerlocation, OUT Playerrotation);
	return Playerlocation;
}