#include "GMASExTemplatePawn.h"

#include "EnhancedInputComponent.h"
#include "GMASExTemplate/Components/GMASExTemplateMovementComponent.h"
#include "Utility/GMASUtilities.h"

AGMASExTemplatePawn::AGMASExTemplatePawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MotionWarpingComponent = ObjectInitializer.CreateDefaultSubobject<UGMCE_MotionWarpingComponent>(this, TEXT("MotionWarpingComponent"));
	OverrideInputComponentClass = UEnhancedInputComponent::StaticClass();
}

void AGMASExTemplatePawn::BeginPlay()
{
	Super::BeginPlay();
	MovementComponent = GetComponentByClass<UGMASExTemplateMovementComponent>();

	if (!IsValid(MovementComponent))
	{
		UE_LOG(LogActor, Error, TEXT("%s has no valid movement component!"), *GetName());
	}
}

USkeletalMeshComponent* AGMASExTemplatePawn::MotionWarping_GetMeshComponent() const
{
	return MeshComponent;
}

float AGMASExTemplatePawn::MotionWarping_GetCollisionHalfHeight() const
{
	if (!MovementComponent) return 0.f;
	
	return MovementComponent->GetRootCollisionHalfHeight(true);
}

FQuat AGMASExTemplatePawn::MotionWarping_GetRotationOffset() const
{
	if (!MeshComponent) return FQuat::Identity;
	
	return MeshComponent->GetRelativeRotation().Quaternion();
}

FVector AGMASExTemplatePawn::MotionWarping_GetTranslationOffset() const
{
	if (!MeshComponent) return FVector::ZeroVector;
	
	return MeshComponent->GetRelativeLocation();
}

FAnimMontageInstance* AGMASExTemplatePawn::GetRootMotionAnimMontageInstance(USkeletalMeshComponent* InMeshComponent) const
{
	return IGMCE_MotionWarpSubject::GetRootMotionAnimMontageInstance(InMeshComponent);
}

UGMCE_OrganicMovementCmp* AGMASExTemplatePawn::GetGMCExMovementComponent() const
{
	return MovementComponent;
}
