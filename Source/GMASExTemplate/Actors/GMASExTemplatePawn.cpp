#include "GMASExTemplatePawn.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GMASExTemplate/Components/GMASExTemplateMovementComponent.h"
#include "Utility/GMASUtilities.h"

AGMASExTemplatePawn::AGMASExTemplatePawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	USceneComponent *TemporaryCapsule = GetComponentByClass<UCapsuleComponent>();

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	MeshComponent->bEditableWhenInherited = true;
	MeshComponent->SetupAttachment(TemporaryCapsule);
	MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	MeshComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->bEditableWhenInherited = true;
	SpringArmComponent->SetupAttachment(TemporaryCapsule);
	SpringArmComponent->TargetArmLength = 400.f;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	CameraComponent->bEditableWhenInherited = true;
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;
	
#if WITH_EDITOR
	UGMASUtilities::SetPropertyFlagsSafe(StaticClass(), TEXT("MeshComponent"), CPF_DisableEditOnInstance);
	UGMASUtilities::SetPropertyFlagsSafe(StaticClass(), TEXT("SpringArmComponent"), CPF_DisableEditOnInstance);
	UGMASUtilities::SetPropertyFlagsSafe(StaticClass(), TEXT("CameraComponent"), CPF_DisableEditOnInstance);
#endif	
}

