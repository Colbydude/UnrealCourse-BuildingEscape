// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *(GetOwner()->GetName()));
	}
}

// "Open" the door.
void UOpenDoor::OpenDoor()
{
	// Create a rotator.
	FRotator NewRotation = FRotator(0.0f, OpenAngle, 0.0f);

	// Set the door rotation.
	if (!Owner) { return; }
	Owner->SetActorRotation(NewRotation);
}

// "Close" the door.
void UOpenDoor::CloseDoor()
{
	// Create a rotator.
	FRotator NewRotation = FRotator(0.0f, 0.0f, 0.0f);

	// Set the door rotation.
	if (!Owner) { return; }
	Owner->SetActorRotation(NewRotation);
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume.
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) {
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// Check if it's time to close the door.
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) {
		CloseDoor();
	}
}

// Returns total mass in kg.
float UOpenDoor::GetTotalMassOfActorsOnPlate() const
{
	float TotalMass = 0.0f;

	// Find all the overlapping actors.
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	
	// Iterate through them adding their masses.
	for (const auto* Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}


