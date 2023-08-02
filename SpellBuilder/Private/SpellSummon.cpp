// Brett Meirhofer 2023


#include "SpellSummon.h"

#include "SpellBuilderCharacter.h"
#include "SpellComponent.h"
#include "WizardAttributeSet.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SpellAbilityComponent.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ASummon::ASummon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<USpellAbilityComponent>(TEXT("AbilitySystemComponent"));
	FGameplayAbilityActorInfo* ActorInfo = new FGameplayAbilityActorInfo();
	ActorInfo->InitFromActor(this, this, AbilitySystemComponent);
	AbilitySystemComponent->AbilityActorInfo = TSharedPtr<FGameplayAbilityActorInfo>(ActorInfo);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AttributeSet = CreateDefaultSubobject<UWizardAttributeSet>(TEXT("AttributeSetBase"));
	SpellComponent = CreateDefaultSubobject<USpellComponent>(TEXT("SpellComponent"));
}

// Called when the game starts or when spawned
void ASummon::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(SearchHandle, this, &ASummon::Search, 1, true);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UWizardAttributeSet::GetSpeedAttribute()).AddUObject(this, &ASummon::SpeedChanged);
	bool Found;
	Cast< UFloatingPawnMovement>(GetComponentByClass(UFloatingPawnMovement::StaticClass()))->MaxSpeed = AbilitySystemComponent->GetGameplayAttributeValue(UWizardAttributeSet::GetSpeedAttribute(), Found);
}

// Called every frame
void ASummon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASummon::Search()
{
	TArray<AActor*> Actors;
	//UKismetSystemLibrary::SphereOverlapActors(this, GetActorLocation(), 1000, { Game }, AActor::StaticClass(), { this }, Actors);
}

void ASummon::SpeedChanged(const FOnAttributeChangeData& Data)
{
	Cast< UFloatingPawnMovement>(GetComponentByClass(UFloatingPawnMovement::StaticClass()))->MaxSpeed = Data.NewValue;
}

FVector ASummon::GetShootLocation_Implementation()
{
	return GetActorLocation();
}

FName ASummon::GetBeamSocket_Implementation()
{
	return FName();
}

FTransform ASummon::GetCameraTransform_Implementation()
{
	return GetActorTransform();
}

USceneComponent* ASummon::GetSocketComponent_Implementation()
{
	return GetRootComponent();
}
