// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "BuildingEscape.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"


#define OUT //annotate the variable that the FUNCTION HAS CHANGED

UPhysicsHandleComponent* PhysicsHandle;



// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	PhysicsHandle->LinearDamping = 200.f;
	PhysicsHandle->LinearStiffness = 750.f;
	PhysicsHandle->AngularDamping = 500.f;
	PhysicsHandle->AngularStiffness = 1500.f;
	PhysicsHandle->InterpolationSpeed = 50.f;
	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();

	
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("GRAB PRESSED"))
	///LINE TRACE and Try and Reach any Actors with PhysicsBody Collision Channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); ///tells us where on the actor the thing hit
	auto ActorHit = HitResult.GetActor(); ///tells us what the thing hit
	///if we hit something, attach a physics handle
	if (ActorHit) {
		if (!PhysicsHandle) {    ///this is to protect physicshandle pointer from crashing system
			return;
		}
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None, //no bones needed
			ActorHit->GetActorLocation(),
			true //allow rotation
		);
	}
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("GRAB RELEASED"))
		PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	UPhysicsHandleComponent* PhysicsHandle = nullptr; ///pointing this to nullpointer as an initializer because we do not know the original value


													  /// Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>(); ///looks for component of type physics comp.
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s missing PhysicsHandleComponent"), *(GetOwner()->GetName()))
	}
}

void UGrabber::SetupInputComponent()
{
	///look for attach input component appears at runtime
	InputHandle = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputHandle) {
			///Bind The InputAction
		InputHandle->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputHandle->BindAction("Grab", IE_Released, this, &UGrabber::Release);

		///IE Pressed means in event pressed, this refers to this component, and the last parameter is a function defined
		///below->this is how you bind the "grab" input to the input handle!

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing Input Handle"), *(GetOwner()->GetName()))
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	///if physics channel is attached
	if (PhysicsHandle->GrabbedComponent) {
		
		/// move the object we are holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd()); ///getreachlineend returns the line trace end
	} 
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() 
{
	//strange because we defined a variable, and the fnc call is changing the variables
	/*
	FString PVPL = PlayerViewPointLocation.ToString();
	FString PVPR = PlayerViewPointRotation.ToString();

	UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *PVPL, *PVPR)*/
	/* draw ray casting out to "reach distance"
	DrawDebugLine(
	GetWorld(),
	PlayerViewPointLocation,
	LineTraceEnd,
	FColor(255, 255, 0),
	false,
	0.f,
	0.f,
	05.f
	);
	*/
	///setup query parameters
	
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner()); ///want the trace to not be complex, and to ignore ourselves

																			   ///line trace out to reach distance
																			   ///we want to detect the line colliding with a specific object type (physicsbody)
	GetWorld()->LineTraceSingleByObjectType(
		OUT LineTraceHit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), //We only want physics body, in the enum collision channel, as seen in the parameters
		TraceParameters
	);

	return LineTraceHit; //the fhitresult
	/*detect "hits"
	AActor* ActorHit = LineTraceHit.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit: %s"), *(ActorHit->GetName())) ///getname yields a string that we must dereference using *
	}*/
	
}

FVector UGrabber::GetReachLineStart()
{
	/// get player viewpoint every tick
	FVector PlayerViewPointLocation;   //defining vector and rotation variables
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}


FVector UGrabber::GetReachLineEnd()
{
	/// get player viewpoint every tick
	FVector PlayerViewPointLocation;   //defining vector and rotation variables
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + Reach * PlayerViewPointRotation.Vector();
	return LineTraceEnd;
}
