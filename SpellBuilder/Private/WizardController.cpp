// Brett Meirhofer 2023


#include "WizardController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "SpellComponent.h"
#include "WizardAttributeSet.h"


// Sets default values
AWizardController::AWizardController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpellComponent = CreateDefaultSubobject<USpellComponent>(TEXT("SpellComponent"));
}

// Called when the game starts or when spawned
void AWizardController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWizardController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWizardController::AddExp(float Amount)
{
	Exp += Amount;
	if (Exp > ExpNeeded)
	{
		Level += 1;
		Exp -= ExpNeeded;
		ExpNeeded *= 1.2;
		UAbilitySystemComponent* Comp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
		Comp->SetNumericAttributeBase(UWizardAttributeSet::GetMaxHealthAttribute(), Comp->GetNumericAttributeBase(UWizardAttributeSet::GetMaxHealthAttribute()) + 20);
		Comp->SetNumericAttributeBase(UWizardAttributeSet::GetMaxManaAttribute(), Comp->GetNumericAttributeBase(UWizardAttributeSet::GetMaxManaAttribute()) + 20);
		Comp->SetNumericAttributeBase(UWizardAttributeSet::GetHealthRegenRateAttribute(), Comp->GetNumericAttributeBase(UWizardAttributeSet::GetHealthRegenRateAttribute()) + 1);
		Comp->SetNumericAttributeBase(UWizardAttributeSet::GetManaRegenRateAttribute(), Comp->GetNumericAttributeBase(UWizardAttributeSet::GetManaRegenRateAttribute()) + 2);
		OnUpdateLevel.Broadcast();
	}
	OnUpdateExp.Broadcast();
}

