// Danny Gillies 2016

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	UPROPERTY(EditAnywhere)
	float Reach = 100.f;
		
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UInputComponent* InputComponent = nullptr;

	// Raycast and grab what's in reach
	void Grab();

	// Release what is grabbed
	void Release();

	// Make sure there is a physics handle component attached to this object
	void FindPhysicsHandleComponent();

	// Setup the attached input component
	void SetupInputComponent();
	
	// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	// Returns current end of reach line
	FVector GetReachLineEnd();

	// Returns current start of reach line
	FVector GetReachLineStart();
};
