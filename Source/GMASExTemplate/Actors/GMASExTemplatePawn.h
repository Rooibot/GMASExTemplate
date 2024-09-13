#pragma once

#include "CoreMinimal.h"
#include "GMASExTemplateMovementComponent.h"
#include "GMASExTemplatePawn_Base.h"
#include "GMCE_MotionWarpingComponent.h"
#include "GMCE_MotionWarpSubject.h"
#include "Actors/GMAS_Pawn.h"
#include "UObject/Object.h"
#include "GMASExTemplatePawn.generated.h"

class UGMASExTemplateMovementComponent;

/**
 * @class AGMASExTemplatePawn
 * @brief A pawn class that extends the base template pawn.
 *
 * This class serves as a third-person convenience pawn with additional components such as a skeletal mesh,
 * a spring arm, and a follow camera.
 */
UCLASS()
class GMASEXTEMPLATE_API AGMASExTemplatePawn : public AGMASExTemplatePawn_Base
{
	GENERATED_BODY()

public:
	explicit AGMASExTemplatePawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", DisplayName="Skeletal Mesh", meta=(AllowPrivateAccess=true))
	TObjectPtr<USkeletalMeshComponent> MeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", DisplayName="Spring Arm", meta=(AllowPrivateAccess=true))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", DisplayName="Follow Camera", meta=(AllowPrivateAccess=true))
	TObjectPtr<UCameraComponent> CameraComponent;

};
