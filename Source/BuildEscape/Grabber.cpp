// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
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


void UGrabber::SetupInputComponent()
{
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
	if (PhysicsHandle)
	{
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot find PhysicsHandle component on %s !!!"), *GetOwner()->GetName());
	}
}


void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Press"));

	FVector Playerlocation;
	FRotator Playerrotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT Playerlocation, OUT Playerrotation);

	FVector LineEnd = Playerlocation + Reach * (Playerrotation.Vector());

	FHitResult HitResult = GetFirstPhysicsBodyInReach();

	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	if (HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None,
			LineEnd
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Realease"));

	PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector Playerlocation;
	FRotator Playerrotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT Playerlocation,OUT Playerrotation);

	FVector LineEnd = Playerlocation + Reach * (Playerrotation.Vector());


	DrawDebugLine(
		GetWorld(),
		Playerlocation,
		LineEnd,
		FColor(125,125,125),
		false,
		0.f,0,5
	);

	FHitResult Hit;
	FCollisionQueryParams TraceParams(*GetOwner()->GetName(),false,GetOwner());
	bool result = GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		Playerlocation,
		LineEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
		);

	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace has hit  : %s"), *ActorHit->GetName());
	}

	return Hit;
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector Playerlocation;
	FRotator Playerrotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT Playerlocation, OUT Playerrotation);

	FVector LineEnd = Playerlocation + Reach * (Playerrotation.Vector());

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineEnd);
	}

}

