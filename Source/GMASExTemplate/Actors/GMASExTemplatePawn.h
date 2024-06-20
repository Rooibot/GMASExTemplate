#pragma once

#include "CoreMinimal.h"
#include "GMASExTemplateMovementComponent.h"
#include "GMCE_MotionWarpingComponent.h"
#include "GMCE_MotionWarpSubject.h"
#include "Actors/GMAS_Pawn.h"
#include "UObject/Object.h"
#include "GMASExTemplatePawn.generated.h"

class UGMASExTemplateMovementComponent;

/**
 * 
 */
UCLASS()
class GMASEXTEMPLATE_API AGMASExTemplatePawn : public AGMAS_Pawn, public IGMCE_MotionWarpSubject
{
	GENERATED_BODY()

public:
	explicit AGMASExTemplatePawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	
	// GMCExtendedAnimation motion warping interface
	virtual USkeletalMeshComponent* MotionWarping_GetMeshComponent() const override;
	virtual float MotionWarping_GetCollisionHalfHeight() const override;
	virtual FQuat MotionWarping_GetRotationOffset() const override;
	virtual FVector MotionWarping_GetTranslationOffset() const override;
	virtual FAnimMontageInstance* GetRootMotionAnimMontageInstance(USkeletalMeshComponent* MeshComponent) const override;
	virtual UGMCE_OrganicMovementCmp* GetGMCExMovementComponent() const override;

protected:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Components")
	UGMASExTemplateMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	UGMCE_MotionWarpingComponent* MotionWarpingComponent;


};
