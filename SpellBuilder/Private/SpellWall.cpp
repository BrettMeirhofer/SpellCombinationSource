// Brett Meirhofer 2023


#include "SpellWall.h"

#include "AbilitySystemComponent.h"
#include "Rune.h"
#include "Spell.h"
#include "SpellComponent.h"
#include "WizardAttributeSet.h"


// Sets default values
ASpellWall::ASpellWall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpellComponent = CreateDefaultSubobject<USpellComponent>(TEXT("SpellComponent"));
}

// Called when the game starts or when spawned
void ASpellWall::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(*SpellComponent->Stats.Find(ESpellStat::Duration));
	SetActorScale3D(FVector(1, *SpellComponent->Stats.Find(ESpellStat::Area), 1));
	AbilitySystemComponent->SetNumericAttributeBase(UWizardAttributeSet::GetHealthAttribute(), *SpellComponent->Stats.Find(ESpellStat::Durability));
	
}

// Called every frame
void ASpellWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

