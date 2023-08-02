// Brett Meirhofer 2023

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "WizardAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UENUM(BlueprintType)
enum class EGDAbilityInputID : uint8
{
	// 0 None
	None			UMETA(DisplayName = "None"),
	// 1 Confirm
	Confirm			UMETA(DisplayName = "Confirm"),
	// 2 Cancel
	Cancel			UMETA(DisplayName = "Cancel"),
	// 3 LMB
	Ability1		UMETA(DisplayName = "Ability1"),
};

// Stores all GAS attributes for project
UCLASS()
class SPELLBUILDER_API UWizardAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	UWizardAttributeSet();
public:
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mana")
		FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UWizardAttributeSet, Mana)

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mana")
		FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UWizardAttributeSet, MaxMana)

		UPROPERTY(BlueprintReadOnly, Category = "Mana")
		FGameplayAttributeData ManaRegenRate;
	ATTRIBUTE_ACCESSORS(UWizardAttributeSet, ManaRegenRate)

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
		FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UWizardAttributeSet, Health)

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
		FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UWizardAttributeSet, MaxHealth)

		UPROPERTY(BlueprintReadOnly, Category = "Health")
		FGameplayAttributeData HealthRegenRate;
	ATTRIBUTE_ACCESSORS(UWizardAttributeSet, HealthRegenRate)

		UPROPERTY(BlueprintReadOnly, Category = "Damage")
		FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UWizardAttributeSet, Damage)

		UPROPERTY(BlueprintReadOnly, Category = "Speed")
		FGameplayAttributeData Speed;
	ATTRIBUTE_ACCESSORS(UWizardAttributeSet, Speed)

		UPROPERTY(BlueprintReadOnly, Category = "Range")
		FGameplayAttributeData SenseRange;
	ATTRIBUTE_ACCESSORS(UWizardAttributeSet, SenseRange)


		void AttributeTick(float DeltaSeconds);

	void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
};
