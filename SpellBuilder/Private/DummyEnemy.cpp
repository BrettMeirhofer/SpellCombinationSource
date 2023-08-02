// Brett Meirhofer 2023


#include "DummyEnemy.h"

#include "AbilitySystemComponent.h"
#include "WizardAttributeSet.h"


// Sets default values
ADummyEnemy::ADummyEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	FGameplayAbilityActorInfo* ActorInfo = new FGameplayAbilityActorInfo();
	ActorInfo->InitFromActor(this, this, AbilitySystemComponent);
	AbilitySystemComponent->AbilityActorInfo = TSharedPtr<FGameplayAbilityActorInfo>(ActorInfo);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AttributeSet = CreateDefaultSubobject<UWizardAttributeSet>(TEXT("AttributeSetBase"));
}

// Called when the game starts or when spawned
void ADummyEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADummyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

