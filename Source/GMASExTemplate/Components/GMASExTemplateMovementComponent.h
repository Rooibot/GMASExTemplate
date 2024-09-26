#pragma once

#include "CoreMinimal.h"
#include "GMCAbilityComponent.h"
#include "GMCE_MotionWarpingComponent.h"
#include "GMCE_OrganicMovementCmp.h"
#include "GameFramework/PlayerStart.h"
#include "UObject/Object.h"
#include "GMASExTemplateMovementComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnRespawn);
 
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

	virtual void MovementUpdate_Implementation(float DeltaSeconds) override;
	
	// GMCEx overrides
	virtual void OnSolverChangedMode(FGameplayTag NewMode, FGameplayTag OldMode) override;

	UFUNCTION(BlueprintCallable, Category = "Respawn")
	virtual void Respawn();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Respawn")
	void GetRespawnLocation(FTransform& OutTransform);

	FOnRespawn OnRespawnDelegate;
	
protected:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Components")
	/// Convenience reference to the GMAS ability system component.
	UGMC_AbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Components")
	/// Convenience reference to the GMCEx motion warping component.
	UGMCE_MotionWarpingComponent* MotionWarpingComponent;

	bool bWantsRespawn;
	bool bShouldRespawn;
	FVector RespawnTargetLocation;
	FRotator RespawnTargetRotation;
};
