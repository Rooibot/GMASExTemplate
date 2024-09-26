#include "GMASExTemplatePawn_Base.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GMASExTemplateMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Utility/GMASUtilities.h"

AGMASExTemplatePawn_Base::AGMASExTemplatePawn_Base(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	// Add our basic goodies.
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(CapsuleComponent);
	CapsuleComponent->SetCapsuleRadius(30.f);
	CapsuleComponent->SetCapsuleHalfHeight(90.f);
	CapsuleComponent->bEditableWhenInherited = true;
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComponent->SetCollisionObjectType(ECC_Pawn);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Block);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	
	AbilitySystemComponent = CreateDefaultSubobject<UGMC_AbilitySystemComponent>(TEXT("Ability Component"));
	MotionWarpingComponent = ObjectInitializer.CreateDefaultSubobject<UGMCE_MotionWarpingComponent>(this, TEXT("MotionWarpingComponent"));
	OverrideInputComponentClass = UEnhancedInputComponent::StaticClass();

#if WITH_EDITOR
	UGMASUtilities::SetPropertyFlagsSafe(StaticClass(), TEXT("CapsuleComponent"), CPF_DisableEditOnInstance);
	UGMASUtilities::SetPropertyFlagsSafe(StaticClass(), TEXT("AbilitySystemComponent"), CPF_DisableEditOnInstance);
	UGMASUtilities::SetPropertyFlagsSafe(StaticClass(), TEXT("MotionWarpingComponent"), CPF_DisableEditOnInstance);
#endif	
}

// Called when the game starts or when spawned
void AGMASExTemplatePawn_Base::BeginPlay()
{
	Super::BeginPlay();
	
	MovementComponent = GetComponentByClass<UGMASExTemplateMovementComponent>();

	if (!IsValid(MovementComponent))
	{
		UE_LOG(LogActor, Error, TEXT("%s has no valid movement component!"), *GetName());
	}

	if (MotionWarpingMeshComponent == nullptr)
	{
		MotionWarpingMeshComponent = GetComponentByClass<USkeletalMeshComponent>();
	}
}

// Called to bind functionality to input
void AGMASExTemplatePawn_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (const UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent); InputMappingContext && EnhancedInput)
	{
		if (const AGMC_PlayerController* Player = Cast<AGMC_PlayerController>(GetController()))
		{
			if (Player->GetLocalPlayer() != nullptr)
			{
				if (UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Player->GetLocalPlayer()))
				{
					// Add our default system as the lowest mapping context.
					InputSystem->AddMappingContext(InputMappingContext, 0);
				}				
			}
		}
	}
	
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGMASExTemplatePawn_Base::Respawn()
{
	if (!MovementComponent) return;

	MovementComponent->OnRespawnDelegate.BindUFunction(this, "OnRespawnCallback");
	MovementComponent->Respawn();
}

void AGMASExTemplatePawn_Base::OnRespawnCallback()
{
	if (!AbilitySystemComponent) return;

	for (const FGameplayTag& ActiveTag : RespawnRemovesActiveTags)
	{
		AbilitySystemComponent->RemoveActiveTag(ActiveTag);
	}

	for (const FGameplayTag& EffectTag : RespawnRemovesEffects)
	{
		AbilitySystemComponent->RemoveEffectByTag(EffectTag, -1, false);
	}

	OnRespawn();
}

void AGMASExTemplatePawn_Base::OnRespawn_Implementation()
{
}

USkeletalMeshComponent* AGMASExTemplatePawn_Base::MotionWarping_GetMeshComponent() const
{
	return MotionWarpingMeshComponent;
}

float AGMASExTemplatePawn_Base::MotionWarping_GetCollisionHalfHeight() const
{
	if (!MovementComponent) return 0.f;
	
	return MovementComponent->GetRootCollisionHalfHeight(true);	
}

FQuat AGMASExTemplatePawn_Base::MotionWarping_GetRotationOffset() const
{
	if (!MotionWarpingMeshComponent) return FQuat::Identity;
	
	return MotionWarpingMeshComponent->GetRelativeRotation().Quaternion();	
}

FVector AGMASExTemplatePawn_Base::MotionWarping_GetTranslationOffset() const
{
	if (!MotionWarpingMeshComponent) return FVector::ZeroVector;

	return MotionWarpingMeshComponent->GetRelativeLocation();
}

FAnimMontageInstance* AGMASExTemplatePawn_Base::GetRootMotionAnimMontageInstance(
	USkeletalMeshComponent* InMeshComponent) const
{
	return IGMCE_MotionWarpSubject::GetRootMotionAnimMontageInstance(InMeshComponent);	
}

UGMCE_OrganicMovementCmp* AGMASExTemplatePawn_Base::GetGMCExMovementComponent() const
{
	return MovementComponent;
}

