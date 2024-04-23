// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/GMCAbilityAnimInstance.h"
#include "GMASExTemplateMovementComponent.h"
#include "GMASExTemplateAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GMASEXTEMPLATE_API UGMASExTemplateAnimInstance : public UGMCAbilityAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	UGMASExTemplateMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	bool bFirstUpdate { true };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State|Movement")
	FRotator ActorRotation { FRotator::ZeroRotator };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State|Movement")
	float ActorYawDeltaRate { 0.f };
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State|Aiming")
	FRotator AimRotation { FRotator::ZeroRotator };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State|Aiming")
	float AimYawDeltaRate { 0.f };
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State|Aiming")
	float AimOffset { 0.f };
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State|Turn-in-Place")
	bool bIsTurningInPlace { false };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State|Turn-in-Place")
	float AimYawRemaining { 0.f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State|Movement")
	FVector WorldVelocity { FVector::ZeroVector };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State|Movement")
	FVector LocalVelocity { FVector::ZeroVector };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State|Movement")
	float LocomotionAngle { 0.f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State|Movement")
	FVector WorldAcceleration { FVector::ZeroVector };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State|Movement")
	FVector LocalAcceleration { FVector::ZeroVector };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State|Input")
	bool bHasInput { false };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State|Input")
	FVector LocalInputDirection { FVector::ZeroVector };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State|Input")
	FVector LocalInputAcceleration { FVector::ZeroVector };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State|Input")
	FVector WorldInputDirection { FVector::ZeroVector };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State|Input")
	FVector WorldInputAcceleration { FVector::ZeroVector };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State|Distance Matching")
	bool bStopPredicted { false };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State|Distance Matching")
	float PredictedStopDistance { 0.f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State|Distance Matching")
	bool bPivotPredicted { false };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State|Distance Matching")
	float PredictedPivotDistance { 0.f };

private:
	void ObtainComponents();
	
};

