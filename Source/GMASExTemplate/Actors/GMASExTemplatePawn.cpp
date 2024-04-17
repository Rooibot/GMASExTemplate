// Fill out your copyright notice in the Description page of Project Settings.


#include "GMASExTemplatePawn.h"

#include "EnhancedInputComponent.h"
#include "GMASExTemplate/Components/GMASExTemplateMovementComponent.h"
#include "Utility/GMASUtilities.h"

AGMASExTemplatePawn::AGMASExTemplatePawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MotionWarpingComponent = CreateDefaultSubobject<UGMCE_MotionWarpingComponent>(TEXT("Motion Warping Component"));
	MovementComponent = CreateDefaultSubobject<UGMASExTemplateMovementComponent>(TEXT("Movement Component"));
	OverrideInputComponentClass = UEnhancedInputComponent::StaticClass();
	
#if WITH_EDITOR
	UGMASUtilities::SetPropertyFlagsSafe(StaticClass(), TEXT("MotionWarpingComponent"), CPF_DisableEditOnInstance);
	UGMASUtilities::SetPropertyFlagsSafe(StaticClass(), TEXT("MovementComponent"), CPF_DisableEditOnInstance);
#endif 	
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
