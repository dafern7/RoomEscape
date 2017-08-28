// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Components/ActorComponent.h"
#include "Gameframework/Actor.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenRequest); //make blueprint assigned (connected) class

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
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

	float GetTotalMassOfActorsOnPlate(); //returns total mass in kilograms

	UPROPERTY(BlueprintAssignable)     //this is for changing the properties of the blueprint door
		FOnOpenRequest OnOpenRequest;		///tells c++ to give power to blueprint to change 
	UPROPERTY(BlueprintAssignable)
		FOnOpenRequest OnClose;

private:

	UPROPERTY(VisibleAnywhere)  //macro that makes the property visible or editable
	float OpenAngle = 90.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* Owner; //owning door defined as a varibale

	UPROPERTY(EditAnywhere)
	float TriggerMass = 50.f;
};
