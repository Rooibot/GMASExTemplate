#pragma once

#include "CoreMinimal.h"
#include "GMCAbilityComponent.h"
#include "GMCE_MotionWarpingComponent.h"
#include "GMCE_OrganicMovementCmp.h"
#include "UObject/Object.h"
#include "GMASExTemplateMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class GMASEXTEMPLATE_API UGMASExTemplateMovementComponent : public UGMCE_OrganicMovementCmp
{
	GENERATED_BODY()

public:

	// GMAS overrides
	virtual void BindReplicationData_Implementation() override;
	virtual void GenAncillaryTick_Implementation(float DeltaTime, bool bLocalMove, bool bCombinedClientMove) override;
	virtual void GenPredictionTick_Implementation(float DeltaTime) override;
	virtual void GenSimulationTick_Implementation(float DeltaTime) override;
	virtual void PreLocalMoveExecution_Implementation(const FGMC_Move& LocalMove) override;

protected:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Components")
	/// Convenience reference to the GMAS ability system component.
	UGMC_AbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Components")
	/// Convenience reference to the GMCEx motion warping component.
	UGMCE_MotionWarpingComponent* MotionWarpingComponent;
	
};
