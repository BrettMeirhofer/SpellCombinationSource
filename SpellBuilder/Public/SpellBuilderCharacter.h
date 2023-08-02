// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SpellInterface.h"
#include "SpellBuilderCharacter.generated.h"

class USpellAbilityComponent;
struct FOnAttributeChangeData;
class UAttributeSet;
class UAbilitySystemComponent;

UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDied);

// The player character. Spellcasting enemies may be derived from this eventually
UCLASS(config=Game)
class ASpellBuilderCharacter : public ACharacter, public ISpellInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

public:
	ASpellBuilderCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpellAbilityComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere)
		UAttributeSet* AttributeSet;

	virtual void Tick(float DeltaSeconds) override;
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual void PossessedBy(AController* NewController) override;

	virtual void Died();

	UPROPERTY(BlueprintReadWrite)
	bool Dead = false;

	virtual void SpeedChanged(const FOnAttributeChangeData& Data);

	UPROPERTY(BlueprintAssignable)
		FDied OnDied;

	virtual FVector GetShootLocation_Implementation() override;

	virtual FName GetBeamSocket_Implementation() override;

	virtual FTransform GetCameraTransform_Implementation() override;

	virtual USceneComponent* GetSocketComponent_Implementation() override;
};


