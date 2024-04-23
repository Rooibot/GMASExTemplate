// Fill out your copyright notice in the Description page of Project Settings.


#include "GMASExTemplateAnimInstance.h"

void UGMASExTemplateAnimInstance::ObtainComponents()
{
	if (!IsValid(GMCPawn) || !IsValid(MovementComponent))
	{
		
#if WITH_EDITOR
		if (!GetWorld()->IsGameWorld())
		{
			// If we're an editor preview, just make sure we have valid data so we don't, y'know, crash.
			if (!GMCPawn) GMCPawn = EditorPreviewClass->GetDefaultObject<AGMC_Pawn>();
			MovementComponent = Cast<UGMASExTemplateMovementComponent>(GMCPawn->GetMovementComponent());

			if (!MovementComponent)
			{
				MovementComponent = NewObject<UGMASExTemplateMovementComponent>();
			}

			return;
		}
#endif
		
		if (!GMCPawn)
		{
			AActor* OwningActor = GetOwningActor();
			GMCPawn = Cast<AGMC_Pawn>(OwningActor);
		}

		if (GMCPawn && !MovementComponent)
		{
			MovementComponent = Cast<UGMASExTemplateMovementComponent>(GMCPawn->GetComponentByClass<UGMCE_OrganicMovementCmp>());
		}
	}	
}


void UGMASExTemplateAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ObtainComponents();	
}

void UGMASExTemplateAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	// We run this here just in case components were added in blueprint and weren't available in initialize animation.
	ObtainComponents();
}

void UGMASExTemplateAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (bFirstUpdate && !MovementComponent && GMCPawn)
	{
		MovementComponent = Cast<UGMASExTemplateMovementComponent>(GMCPawn->GetMovementComponent());
	}

	// No component, nothing we can do.
	if (!MovementComponent) return;

	WorldVelocity = MovementComponent->GetLinearVelocity_GMC();
	WorldAcceleration = MovementComponent->GetCurrentAnimationAcceleration();

	AimRotation = MovementComponent->GetCurrentAimRotation();
	AimYawDeltaRate = MovementComponent->GetCurrentAimYawRate();

	ActorRotation = MovementComponent->GetCurrentComponentRotation();
	ActorYawDeltaRate = MovementComponent->GetCurrentComponentYawRate();

	AimYawRemaining = MovementComponent->GetAimYawRemaining();
	
	const FVector InputVector = MovementComponent->GetRawInputVector();
	const FVector InputDirection = InputVector.GetSafeNormal();
	const FVector InputAcceleration = InputVector * MovementComponent->GetInputAcceleration();

	bIsTurningInPlace = (WorldVelocity.IsNearlyZero() && AimYawRemaining != 0.f);

	bHasInput = MovementComponent->IsInputPresent();

	FVector Prediction;
	bStopPredicted = MovementComponent->IsStopPredicted(Prediction);
	PredictedStopDistance = bStopPredicted ? Prediction.Length() : 0.f;

	bPivotPredicted = MovementComponent->IsPivotPredicted(Prediction);
	PredictedPivotDistance = bPivotPredicted ? Prediction.Length() : 0.f;

	if (GetWorld()->IsGameWorld())
	{
		const auto ComponentTransform = GMCPawn->GetActorTransform();
		LocalVelocity = ComponentTransform.InverseTransformVector(WorldVelocity);
		LocalAcceleration = ComponentTransform.InverseTransformVector(WorldAcceleration);
		WorldInputDirection = MovementComponent->GetControllerRotation_GMC().RotateVector(InputDirection);
		WorldInputAcceleration = MovementComponent->GetControllerRotation_GMC().RotateVector(InputAcceleration);
		LocalInputDirection = ComponentTransform.InverseTransformVector(WorldInputDirection);
		LocalInputAcceleration = ComponentTransform.InverseTransformVector(WorldInputAcceleration);
	}
	else
	{
		// We're an editor preview, there's no actual world transform to bother with.
		LocalVelocity = WorldVelocity;
		LocalAcceleration = WorldAcceleration;
		WorldInputDirection = LocalInputDirection = InputDirection;
		WorldInputAcceleration = LocalInputAcceleration = InputAcceleration;
	}

	
	bFirstUpdate = false;
}
