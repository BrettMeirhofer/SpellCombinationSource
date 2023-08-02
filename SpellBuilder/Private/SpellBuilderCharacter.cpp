// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpellBuilderCharacter.h"

#include "AIController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SpellAbilityComponent.h"
#include "WizardAttributeSet.h"


//////////////////////////////////////////////////////////////////////////
// ASpellBuilderCharacter

ASpellBuilderCharacter::ASpellBuilderCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	AbilitySystemComponent = CreateDefaultSubobject<USpellAbilityComponent>(TEXT("AbilitySystemComponent"));
	FGameplayAbilityActorInfo* ActorInfo = new FGameplayAbilityActorInfo();
	ActorInfo->InitFromActor(this, this, AbilitySystemComponent);
	AbilitySystemComponent->AbilityActorInfo = TSharedPtr<FGameplayAbilityActorInfo>(ActorInfo);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AttributeSet = CreateDefaultSubobject<UWizardAttributeSet>(TEXT("AttributeSetBase"));
}

void ASpellBuilderCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UWizardAttributeSet::GetSpeedAttribute()).AddUObject(this, &ASpellBuilderCharacter::SpeedChanged);
	bool Found;
	GetCharacterMovement()->MaxWalkSpeed = AbilitySystemComponent->GetGameplayAttributeValue(UWizardAttributeSet::GetSpeedAttribute(), Found);
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ASpellBuilderCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void ASpellBuilderCharacter::Died()
{
	if (Dead)
		return;

	if (GetController())
	{
		GetController()->UnPossess();
	}
	GetCapsuleComponent()->SetEnableGravity(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AbilitySystemComponent->RemoveActiveEffects(FGameplayEffectQuery());
	Dead = true;
	SetLifeSpan(5);
	GetMesh()->SetCollisionProfileName("RagDoll");
	GetMesh()->SetSimulatePhysics(true);
}

void ASpellBuilderCharacter::SpeedChanged(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

FVector ASpellBuilderCharacter::GetShootLocation_Implementation()
{
	return GetMesh()->GetSocketTransform("hand_r").GetLocation();
}

FName ASpellBuilderCharacter::GetBeamSocket_Implementation()
{
	return FName("hand_r");
}

FTransform ASpellBuilderCharacter::GetCameraTransform_Implementation()
{
	FVector CamLoc = Cast<APlayerController>(GetController())->PlayerCameraManager->GetCameraLocation();
	FRotator CamRot = GetControlRotation();
	FTransform Out = FTransform(CamRot, CamLoc, FVector(1, 1, 1));
	return Out;
}

USceneComponent* ASpellBuilderCharacter::GetSocketComponent_Implementation()
{
	return GetMesh();
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASpellBuilderCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASpellBuilderCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASpellBuilderCharacter::Look);

		FTopLevelAssetPath AbilityEnumAssetPath = FTopLevelAssetPath(FName("/Script/SpellBuilder"), FName("EGDAbilityInputID"));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), AbilityEnumAssetPath, static_cast<int32>(EGDAbilityInputID::Ability1), static_cast<int32>(EGDAbilityInputID::Cancel)));

	}

}

void ASpellBuilderCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ASpellBuilderCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASpellBuilderCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}




