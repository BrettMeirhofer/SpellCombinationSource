// Brett Meirhofer 2023


#include "SpellBeam.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Rune.h"
#include "SpellComponent.h"
#include "TraceHelpers.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ASpellBeam::ASpellBeam()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BeamEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	SpellComponent = CreateDefaultSubobject<USpellComponent>(TEXT("SpellComponent"));
}

// Called when the game starts or when spawned
void ASpellBeam::BeginPlay()
{
	Super::BeginPlay();
	Length = *SpellComponent->Stats.Find(ESpellStat::Range);
	float* PierceValue = SpellComponent->Stats.Find(ESpellStat::Piercing);
	if (PierceValue)
	{
		Pierce = *PierceValue;
	}
	GetWorldTimerManager().SetTimer(DamageTimer, this, &ASpellBeam::ApplyDamage, DamageInterval, true);
	
}

void ASpellBeam::ApplyDamage()
{
	for (AActor* Target : Targets)
	{
		if (!Target)
		{
			continue;
		}
		UAbilitySystemComponent* AbilityComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
		if (!AbilityComp)
		{
			continue;
		}

		for (FGameplayEffectSpecHandle Spec : SpellComponent->EffectHandles)
		{
			AbilityComp->BP_ApplyGameplayEffectSpecToSelf(Spec);
		}
	}
}

// Called every frame
void ASpellBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FTransform CamTransform = ISpellInterface::Execute_GetCameraTransform(GetOwner());
	Targets.Empty();
	TArray<AActor*> Ignore = { this, GetOwner() };
	TArray<FVector> Locs = {};
	USpellHelperFunctionLibrary::BeamTrace(this, CamTransform, GetActorLocation(), Length, 0, 0, 0, Locs, Targets, Ignore);
	BeamEffect->SetVectorParameter("User.BeamEnd", Locs.Last());
}
