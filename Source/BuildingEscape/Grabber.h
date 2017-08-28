// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



private:
	

	/*FString PVPL;
	FString PVPR;*/
	float Reach = 100.f;
	FHitResult LineTraceHit;
	UInputComponent* InputHandle = nullptr; //input component pointer
	void Grab(); //Ray-Cast and grab whats in reach function prototype defined in cpp file
	void Release(); //release
	void FindPhysicsHandleComponent(); //finds attached physics handle prototype
	void SetupInputComponent(); //Setup attached Input Component
	FHitResult GetFirstPhysicsBodyInReach();   //Return hit for first physics body on reach
	FVector GetReachLineStart();
	FVector GetReachLineEnd(); //usesplayer viewpoint location and stuff to get the end of the vector
	UPROPERTY(VisibleDefaultsOnly, Category = PhysicHandle)
		class UPhysicsHandleComponent* PhysicsHandle;
};
