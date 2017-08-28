// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Gameframework/Actor.h"

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
	Owner = GetOwner(); //private variable (global)
	//protection line against crash
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *GetOwner()->GetName())
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// poll the trigger volume (is it pressed) every frame
	// if ActorThatOpens is in the volume, then open door

	if (GetTotalMassOfActorsOnPlate() > TriggerMass) { 
		OnOpenRequest.Broadcast();
	}
	else {
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	float TotalMass = 0.f; //intializing totalmass

	
	TArray<AActor*> OverlappingActors; //giving me an array of actor pointers


	if (!PressurePlate) { return TotalMass; }


	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	//iterate through them, adding masses
	for (const auto* Actor : OverlappingActors) {
		///the component with mass is a uprimitive component
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate!"), *Actor->GetName())
	}

	return TotalMass;
}

