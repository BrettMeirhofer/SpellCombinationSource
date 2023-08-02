// Brett Meirhofer 2023


#include "SpellProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Spell.h"
#include "Rune.h"
#include "SpellComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ASpellProjectile::ASpellProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileComponent->InitialSpeed = 0;
	SpellComponent = CreateDefaultSubobject<USpellComponent>(TEXT("SpellComponent"));
}

// Called when the game starts or when spawned
void ASpellProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASpellProjectile::BeginOverlap);
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	TArray<UActorComponent*> Comps;
	GetOwner()->GetComponents(Comps);
	for (UActorComponent* Comp: Comps)
	{
		UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(Comp);
		if (Prim)
		{
			Prim->IgnoreActorWhenMoving(this, true);
		}
	}

	float* Homing = SpellComponent->Stats.Find(ESpellStat::Homing);
	if (Homing)
	{
		HomingArea *= *Homing;
		ProjectileComponent->bIsHomingProjectile = true;
		ProjectileComponent->HomingAccelerationMagnitude = HomingAccel * *Homing;
		FTimerHandle Handle;
		GetWorldTimerManager().SetTimer(Handle, this, &ASpellProjectile::HomingSearch, .1, true);
	}
	Area = *SpellComponent->Stats.Find(ESpellStat::Area);
	float Speed = *SpellComponent->Stats.Find(ESpellStat::Speed);
	ProjectileComponent->InitialSpeed = Speed;
	ProjectileComponent->MaxSpeed = Speed;
	USkeletalMeshComponent* Skel = Cast< USkeletalMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	FVector Start;
	if (Skel)
	{
		Start = Skel->GetSocketLocation("hand_r");
	}
	else
	{
		Start = GetOwner()->GetActorLocation();
	}

	FTransform CamTransform = ISpellInterface::Execute_GetCameraTransform(GetOwner());
	FVector Cam = CamTransform.GetLocation();
	FRotator LookRot = CamTransform.GetRotation().Rotator();
	FVector Forward = UKismetMathLibrary::GetForwardVector(LookRot) * 10000;
	FVector End = Cam + Forward;
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(Start, End);
	ProjectileComponent->Velocity = UKismetMathLibrary::GetForwardVector(Rot) * Speed;
	SetActorEnableCollision(true);
}

// Called every frame
void ASpellProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpellProjectile::ExplodeDamage()
{
	TArray<AActor*> Actors;
	TArray<TEnumAsByte<EObjectTypeQuery>> Query = { EObjectTypeQuery::ObjectTypeQuery1, EObjectTypeQuery::ObjectTypeQuery2, EObjectTypeQuery::ObjectTypeQuery3 };
	UKismetSystemLibrary::SphereOverlapActors(this, GetActorLocation(), Area, Query, AActor::StaticClass(), {this}, Actors);
	for (AActor* Target: Actors)
	{
		UAbilitySystemComponent* AbilityComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
		if (!AbilityComp)
		{
			continue;
		}

		for (FGameplayEffectSpecHandle Spec: SpellComponent->EffectHandles)
		{
			AbilityComp->BP_ApplyGameplayEffectSpecToSelf(Spec);
		}
	}
}

void ASpellProjectile::BeginOverlap(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	int BodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	if (Other != GetInstigator())
	{
		ExplodeDamage();
		OnExplode.Broadcast();
		Destroy();
	}
}



void ASpellProjectile::HomingSearch()
{
	if(!ProjectileComponent->HomingTargetComponent.Get())
	{
		TArray<AActor*> Out;
		UKismetSystemLibrary::SphereOverlapActors(this, GetActorLocation(), HomingArea, 
			{UEngineTypes::ConvertToObjectType(ECC_Pawn)}, AActor::StaticClass(), { this }, Out);
		Out = USpellHelperFunctionLibrary::FilterByEnemyTeam(Out, SpellComponent->Team);
		if (Out.Num() > 0)
		{
			Out = USpellHelperFunctionLibrary::SortByDistance(Out, this);
			ProjectileComponent->HomingTargetComponent = Out[0]->GetRootComponent();
		}
	}
}