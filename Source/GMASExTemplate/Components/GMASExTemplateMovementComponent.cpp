#include "GMASExTemplateMovementComponent.h"

#include "GMCExtendedLog.h"
#include "Engine/PackageMapClient.h"
#include "Kismet/GameplayStatics.h"

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

	BindBool(
		bWantsRespawn,
		EGMC_PredictionMode::ClientAuth_Input,
		EGMC_CombineMode::CombineIfUnchanged,
		EGMC_SimulationMode::PeriodicAndOnChange_Output,
		EGMC_InterpolationFunction::NearestNeighbour);

	BI_ShouldRespawn = BindBool(
		bShouldRespawn,
		EGMC_PredictionMode::ServerAuth_Output_ClientValidated,
		EGMC_CombineMode::CombineIfUnchanged,
		EGMC_SimulationMode::PeriodicAndOnChange_Output,
		EGMC_InterpolationFunction::NearestNeighbour
		);

	BindBool(
		bRespawnReady,
		EGMC_PredictionMode::ClientAuth_Input,
		EGMC_CombineMode::CombineIfUnchanged,
		EGMC_SimulationMode::None,
		EGMC_InterpolationFunction::NearestNeighbour);
	
	BindCompressedVector(
		RespawnTargetLocation,
		EGMC_PredictionMode::ServerAuth_Output_ClientValidated,
		EGMC_CombineMode::CombineIfUnchanged,
		EGMC_SimulationMode::None,
		EGMC_InterpolationFunction::NearestNeighbour);

	BindCompressedRotator(
		RespawnTargetRotation,
		EGMC_PredictionMode::ServerAuth_Output_ClientValidated,
		EGMC_CombineMode::CombineIfUnchanged,
		EGMC_SimulationMode::None,
		EGMC_InterpolationFunction::NearestNeighbour);
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

void UGMASExTemplateMovementComponent::SV_PreRemoteMoveExecution_Implementation(const FGMC_Move& RemoteMove)
{
	Super::SV_PreRemoteMoveExecution_Implementation(RemoteMove);

	if (AbilitySystemComponent) AbilitySystemComponent->PreRemoteMoveExecution();
}

void UGMASExTemplateMovementComponent::MovementUpdate_Implementation(float DeltaSeconds)
{
	Super::MovementUpdate_Implementation(DeltaSeconds);

	if (bShouldRespawn && bRespawnReady)
	{
		bWantsRespawn = false;
		bShouldRespawn = false;
		bRespawnReady = false;
		if (GetMovementMode() == GetRagdollMode())
		{
			DisableRagdoll();
		}
		
		UpdatedComponent->SetWorldLocationAndRotation(RespawnTargetLocation, RespawnTargetRotation, false, nullptr, ETeleportType::ResetPhysics);
		OnRespawnDelegate.Execute();
	}
	else if (bWantsRespawn && !bShouldRespawn && GetOwnerRole() == ROLE_Authority)
	{
		// A respawn has been requested by the client; we should handle it.
		Respawn_Internal();
	}
}

void UGMASExTemplateMovementComponent::OnSolverChangedMode(FGameplayTag NewMode, FGameplayTag OldMode)
{
	Super::OnSolverChangedMode(NewMode, OldMode);

	if (AbilitySystemComponent)
	{
		// Put our active solver into GMAS as an active tag, to make it easier to key abilities off of.
		if (OldMode != FGameplayTag::EmptyTag)
		{
			AbilitySystemComponent->RemoveActiveTag(OldMode);
		}

		if (NewMode != FGameplayTag::EmptyTag)
		{
			AbilitySystemComponent->AddActiveTag(NewMode);
		}
	}

}

void UGMASExTemplateMovementComponent::Respawn()
{
	if (GetOwnerRole() == ROLE_SimulatedProxy) return;

	if (GetOwnerRole() == ROLE_Authority)
	{
		// Kick off the respawn logic.
		Respawn_Internal();
	}
	else
	{
		// We're an autonomous proxy, so flag the server that we'd like to respawn.
		bWantsRespawn = true;
	}
}

void UGMASExTemplateMovementComponent::Respawn_Internal()
{
	FTransform RespawnTransform;
	GetRespawnLocation(RespawnTransform);

	if (GetNetMode() == NM_Standalone)
	{
		// We're standalone, so we're good to go.
		bRespawnReady = true;	
	}
	else 
	{
		// We're in multiplayer. To make sure we respawn in the same location, send our selected
		// respawn point to the client.
		CL_SetRespawnLocation(RespawnTransform);
	}
	RespawnTargetLocation = RespawnTransform.GetLocation();
	RespawnTargetRotation = RespawnTransform.Rotator();
	bShouldRespawn = true;
}

void UGMASExTemplateMovementComponent::CL_SetRespawnLocation_Implementation(const FTransform& TargetTransform)
{
	// Receive a selected respawn point from the authority, and mark us ready to go.
	RespawnTargetLocation = TargetTransform.GetLocation();
	RespawnTargetRotation = TargetTransform.Rotator();
	bShouldRespawn = true;
	bRespawnReady = true;
}

void UGMASExTemplateMovementComponent::GetRespawnLocation_Implementation(FTransform& OutTransform)
{
	TArray<AActor*> Actors;
	
	// Basic default; grab a random APlayerStart.
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), Actors);
	if (Actors.Num() == 0)
	{
		OutTransform = GetActorTransform_GMC();
		return;
	}

	int ActorToUse = FMath::RandRange(0, Actors.Num() - 1);
	const AActor *Actor = Actors[ActorToUse];
	OutTransform = Actor->GetActorTransform();
}
