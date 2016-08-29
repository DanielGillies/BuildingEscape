// Danny Gillies 2016

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

// Look for attached input component and set it up
void UGrabber::SetupInputComponent()
{
	/// Look for attached input component (only appears at runtime)
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		/// InputComponent is found
		/// Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		/// No physics handle found
		UE_LOG(LogTemp, Error, TEXT("ERROR: UNABLE TO FIND INPUT COMPONENT ON %s"), *GetOwner()->GetName());
	}
}

// Look for attached physics handle
void UGrabber::FindPhysicsHandleComponent()
{
	/// Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		// No physics handle found
		UE_LOG(LogTemp, Error, TEXT("ERROR: UNABLE TO FIND PHYSICS HANDLE ON %s"), *GetOwner()->GetName());
	}
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (!PhysicsHandle) { return; }
	/// If the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		/// Move the object that we're holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}

}

void UGrabber::Grab()
{	
	/// Line trace and see if we reach any actors with phyisics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); /// Gets mesh of object
	auto ActorHit = HitResult.GetActor();

	/// If we hit something, then attach a physics handle
	if (ActorHit)
	{
		if (!PhysicsHandle) { return; }
		/// Attach physics handle
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}
}

void UGrabber::Release()
{
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Line-trace AKA Ray-cast out to reach distance
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(OUT HitResult, 
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	AActor* ActorHit = HitResult.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit %s"), *ActorHit->GetName());
	}
	return HitResult;
}

FVector UGrabber::GetReachLineEnd()
{
	/// Get player viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	/// Calculate the end of the vector for the user's reach
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

FVector UGrabber::GetReachLineStart()
{
	/// Get player viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	/// Calculate the end of the vector for the user's reach
	return PlayerViewPointLocation;
}