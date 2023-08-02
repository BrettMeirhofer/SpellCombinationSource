// Brett Meirhofer 2023


#include "SpellAbilityComponent.h"

#include "WizardAttributeSet.h"



// Sets default values for this component's properties
USpellAbilityComponent::USpellAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	

	// ...
}


// Called when the game starts
void USpellAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UGameplayAbility> Ability : DefaultAbilities)
	{
		K2_GiveAbility(Ability);
	}

	for (TSubclassOf<UGameplayEffect> Effect : DefaultEffects)
	{
		BP_ApplyGameplayEffectToSelf(Effect, 0, MakeEffectContext());
	}
	
}


// Called every frame
void USpellAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

