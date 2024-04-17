// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GMCE_MotionWarpingComponent.h"
#include "GMCE_MotionWarpSubject.h"
#include "Actors/GMAS_Pawn.h"
#include "UObject/Object.h"
#include "GMASExTemplatePawn.generated.h"

/**
 * 
 */
UCLASS()
class GMASEXTEMPLATE_API AGMASExTemplatePawn : public AGMAS_Pawn, public IGMCE_MotionWarpSubject
{
	GENERATED_BODY()

public:
	AGMASExTemplatePawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// GMCExtendedAnimation motion warping interface
	virtual USkeletalMeshComponent* MotionWarping_GetMeshComponent() const override;
	virtual float MotionWarping_GetCollisionHalfHeight() const override;
	virtual FQuat MotionWarping_GetRotationOffset() const override;
	virtual FVector MotionWarping_GetTranslationOffset() const override;
	virtual FAnimMontageInstance* GetRootMotionAnimMontageInstance(USkeletalMeshComponent* MeshComponent) const override;
	virtual UGMCE_OrganicMovementCmp* GetGMCExMovementComponent() const override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	UGMCE_OrganicMovementCmp* MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	UGMCE_MotionWarpingComponent* MotionWarpingComponent;
};
