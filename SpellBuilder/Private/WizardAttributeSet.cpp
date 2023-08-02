// Brett Meirhofer 2023


#include "WizardAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "SpellBuilderCharacter.h"
#include "SpellComponent.h"
#include "WizardController.h"

UWizardAttributeSet::UWizardAttributeSet()
{
	MaxMana = 100;
	Mana = MaxMana;
	ManaRegenRate = 10;

	MaxHealth = 100;
	Health = MaxHealth;
	HealthRegenRate = 1;

	Speed = 600;
	SenseRange = 500;
}

void UWizardAttributeSet::AttributeTick(float DeltaSeconds)
{

	if (Health.GetCurrentValue() < MaxHealth.GetCurrentValue())
	{
		SetHealth(FMath::Clamp(Health.GetCurrentValue() + (HealthRegenRate.GetCurrentValue() * DeltaSeconds), 0.f, MaxHealth.GetCurrentValue()));
	}

	if (Mana.GetCurrentValue() < MaxMana.GetCurrentValue())
	{
		SetMana(FMath::Clamp(Mana.GetCurrentValue() + (ManaRegenRate.GetCurrentValue() * DeltaSeconds), 0.f, MaxMana.GetCurrentValue()));
	}
}

void UWizardAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	UAbilitySystemComponent* TargetComp = Data.Target.AbilityActorInfo->AbilitySystemComponent.Get();

	if (!IsValid(TargetComp))
		return;

	AWizardController* Player = Cast< AWizardController>(TargetComp->GetWorld()->GetFirstPlayerController());
	AActor* TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();

	if (!IsValid(TargetActor))
	{
		return;
	}

	if (TargetComp->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead")))
	{
		return;
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		float NewHealth = FMath::Clamp(GetHealth() - GetDamage(), 0, GetMaxHealth());
		float DamageDone = GetHealth() - NewHealth;
		SetDamage(0);
		SetHealth(NewHealth);
		FGameplayTagContainer Tags;
		Data.EffectSpec.GetAllAssetTags(Tags);
		Player->ShowDamageNumber(DamageDone, TargetActor->GetActorLocation(), Tags);

		if (GetHealth() > 0)
			return;

		TargetComp->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead"));

		if (!Source)
			return;

		FName SourceTeam = Cast< USpellComponent>(Source->GetAvatarActor()->GetComponentByClass(USpellComponent::StaticClass()))->Team;
		if (SourceTeam != FName("Player"))
			return;

		Player->Kills += 1;
		Player->AddExp(50);
		Player->OnUpdateKills.Broadcast();
	}
}

void UWizardAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// Prevent resource from going above max-resource
	if (Attribute == GetHealthAttribute())
	{
		if (NewValue > GetMaxHealth())
		{
			NewValue = GetMaxHealth();
		}
	}

	if (Attribute == GetManaAttribute())
	{
		if (NewValue > GetMaxMana())
		{
			NewValue = GetMaxMana();
		}
	}

	// If resource is full and max resource changes set resource to max-resource
	if (Attribute == GetMaxHealthAttribute())
	{
		if (GetMaxHealth() == GetHealth())
		{
			SetHealth(NewValue);
		}
	}

	if (Attribute == GetMaxManaAttribute())
	{
		if (GetMaxMana() == GetMana())
		{
			SetMana(NewValue);
		}
	}
}
