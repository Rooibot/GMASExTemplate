// Fill out your copyright notice in the Description page of Project Settings.


#include "GMASExTemplateMovementComponent.h"

void UGMASExTemplateMovementComponent::BindReplicationData_Implementation()
{
	Super::BindReplicationData_Implementation();

	AbilitySystemComponent = Cast<UGMC_AbilitySystemComponent>(GetGMCPawnOwner()->GetComponentByClass(UGMC_AbilitySystemComponent::StaticClass()));
	MotionWarpingComponent = Cast<UGMCE_MotionWarpingComponent>(GetGMCPawnOwner()->GetComponentByClass(UGMCE_MotionWarpingComponent::StaticClass()));

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GMCMovementComponent = this;
		AbilitySystemComponent->BindReplicationData();
	}
}

void UGMASExTemplateMovementComponent::GenAncillaryTick_Implementation(float DeltaTime, bool bLocalMove,
	bool bCombinedClientMove)
{
	Super::GenAncillaryTick_Implementation(DeltaTime, bLocalMove, bCombinedClientMove);

	if (AbilitySystemComponent) AbilitySystemComponent->GenAncillaryTick(DeltaTime, bCombinedClientMove);
}

void UGMASExTemplateMovementComponent::GenPredictionTick_Implementation(float DeltaTime)
{
	Super::GenPredictionTick_Implementation(DeltaTime);

	if (AbilitySystemComponent) AbilitySystemComponent->GenPredictionTick(DeltaTime);
}

void UGMASExTemplateMovementComponent::GenSimulationTick_Implementation(float DeltaTime)
{
	Super::GenSimulationTick_Implementation(DeltaTime);

	if (AbilitySystemComponent) AbilitySystemComponent->GenSimulationTick(DeltaTime);
}

void UGMASExTemplateMovementComponent::PreLocalMoveExecution_Implementation(const FGMC_Move& LocalMove)
{
	Super::PreLocalMoveExecution_Implementation(LocalMove);

	if (AbilitySystemComponent) AbilitySystemComponent->PreLocalMoveExecution();
}
