#include "GMASExTemplateMovementComponent.h"

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
		EGMC_SimulationMode::None,
		EGMC_InterpolationFunction::NearestNeighbour);

	BindBool(
		bShouldRespawn,
		EGMC_PredictionMode::ServerAuth_Output_ClientValidated,
		EGMC_CombineMode::CombineIfUnchanged,
		EGMC_SimulationMode::None,
		EGMC_InterpolationFunction::NearestNeighbour
		);
	
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

void UGMASExTemplateMovementComponent::MovementUpdate_Implementation(float DeltaSeconds)
{
	Super::MovementUpdate_Implementation(DeltaSeconds);

	if (bShouldRespawn)
	{
		bWantsRespawn = false;
		bShouldRespawn = false;
		if (GetMovementMode() == GetRagdollMode())
		{
			DisableRagdoll();
		}
		SetActorLocationAndRotation_GMC(RespawnTargetLocation, RespawnTargetRotation, false);
		OnRespawnDelegate.Execute();
	}
	
	if (bWantsRespawn)
	{
		FTransform RespawnTransform;
		GetRespawnLocation(RespawnTransform);
		RespawnTargetLocation = RespawnTransform.GetLocation();
		RespawnTargetRotation = RespawnTransform.Rotator();
		bShouldRespawn = true;
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
	bWantsRespawn = true;
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

	int RandomIndex = FMath::RandRange(0, Actors.Num() - 1);
	const AActor *Actor = Actors[RandomIndex];
	OutTransform = Actor->GetActorTransform();
}
