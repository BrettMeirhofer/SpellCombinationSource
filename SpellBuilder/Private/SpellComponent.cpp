// Brett Meirhofer 2023


#include "SpellComponent.h"
#include "Rune.h"
#include "Spell.h"
#include "AbilitySystemComponent.h"
#include "TraceHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
USpellComponent::USpellComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpellComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<USpell> SpellClass: DefaultSpells)
	{
		USpell* Spell = NewObject<USpell>(this, SpellClass);
		Spell->CacheProps();
		SpellList.Add(Spell);
	}
}


// Called every frame
void USpellComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

USpell* USpellComponent::GetCurrentSpell()
{
	if(!SpellList.IsValidIndex(SelectedSpell))
	{
		return nullptr;
	}
	return SpellList[SelectedSpell];
}

TSubclassOf<UGameplayAbility> USpellComponent::GetCurrentAbility()
{
	return Cast<UForm>(GetCurrentSpell()->SubSpells[0].Runes[0].GetDefaultObject())->Ability;
}

AActor* USpellHelperFunctionLibrary::BP_SpawnActorDeferred(AActor* Context, TSubclassOf<AActor> Class, FTransform Transform,
                                                           AActor* Owner, APawn* Instigator, ESpawnActorCollisionHandlingMethod Collision, ESpawnActorScaleMethod ScaleMethod)
{
	if (!Context)
		return nullptr;

	AActor* Out = Context->GetWorld()->SpawnActorDeferred<AActor>(Class, Transform, Owner, Instigator, Collision, ScaleMethod);
	return Out;
}

void USpellHelperFunctionLibrary::FinishSpawningActor(AActor* Actor, FTransform Transform)
{
	if (!Actor)
		return;

	Actor->FinishSpawning(Transform);
}

TArray<AActor*> USpellHelperFunctionLibrary::FilterByEnemyTeam(const TArray<AActor*>& Array, FName Team)
{ 
	TArray<AActor*> OutArray;
	for (AActor* Actor: Array)
	{
		USpellComponent* SpellComp = Cast< USpellComponent>(Actor->GetComponentByClass(USpellComponent::StaticClass()));
		if (!SpellComp)
			continue;

		if (SpellComp->Team == Team)
			continue;

		OutArray.Add(Actor);
	}
	return OutArray;
}

TArray<AActor*> USpellHelperFunctionLibrary::FilterByAllyTeam(const TArray<AActor*>& Array, FName Team)
{
	TArray<AActor*> OutArray;
	for (AActor* Actor : Array)
	{
		USpellComponent* SpellComp = Cast< USpellComponent>(Actor->GetComponentByClass(USpellComponent::StaticClass()));
		if (!SpellComp)
			continue;

		if (SpellComp->Team != Team)
			continue;

		OutArray.Add(Actor);
	}
	return OutArray;
}

TArray<AActor*> USpellHelperFunctionLibrary::SortByDistance(TArray<AActor*> Array, AActor* Target)
{
	Array.Sort([Target](const AActor& a, const AActor& b)
	{
		return a.GetSquaredDistanceTo(Target) < b.GetSquaredDistanceTo(Target);
	});
	//Array.Sort([](const AActor* a, const AActor* b) { return a == b; });
	return Array;
}

void USpellHelperFunctionLibrary::BeamTrace(AActor* Context, FTransform CamTransform, FVector SourceLoc, float Length, int Bounces, float SeekDistance, int Pierces, TArray<FVector>& Locs, TArray<AActor*>& Actors, const TArray<AActor*>& Ignore)
{
	FVector Cam = CamTransform.GetLocation();
	FRotator LookRot = CamTransform.GetRotation().Rotator();
	FVector Forward = UKismetMathLibrary::GetForwardVector(LookRot) * Length;
	FVector End = Cam + Forward;
	FHitResult Result;
	if (Cam != SourceLoc)
	{
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
		UKismetSystemLibrary::LineTraceSingleForObjects(Context, CamTransform.GetLocation(), End, ObjectTypes, false, Ignore, EDrawDebugTrace::None, Result,false);
		//UTraceHelpers::LineTraceByObjectChannel(Context, Result, CamTransform.GetLocation(), End, ECC_GameTraceChannel1, Ignore);
		if (Result.bBlockingHit)
		{
			End = Result.ImpactPoint;
		}
	}
	TArray<AActor*> IgnoreMut = { Ignore };
	for (int i = 0; i < Pierces; i++)
	{
		bool Hit = UTraceHelpers::LineTraceByObjectChannel(Context, Result, SourceLoc, End, ECC_GameTraceChannel1, IgnoreMut);

		if (Hit)
		{
			Actors.Add(Result.GetActor());
			IgnoreMut.Add(Result.GetActor());
			//End = Result.ImpactPoint;
			Locs.Add(Result.ImpactPoint);
		} else
		{
			Locs.Add(End);
			// Pierced through everything
			break;
		}
	}
}

bool USpellHelperFunctionLibrary::IsActiveEffectHandleValid(FActiveGameplayEffectHandle Handle)
{
	if (!Handle.GetOwningAbilitySystemComponent())
		return false;

	const FActiveGameplayEffect* Effect = Handle.GetOwningAbilitySystemComponent()->GetActiveGameplayEffect(Handle);
	return Effect != nullptr;
}

float USpellHelperFunctionLibrary::GetTimeRemainingEffect(FActiveGameplayEffectHandle Handle, float WorldTime)
{
	const FActiveGameplayEffect* Effect = Handle.GetOwningAbilitySystemComponent()->GetActiveGameplayEffect(Handle);
	return Effect->GetTimeRemaining(WorldTime);
}

TSubclassOf<UGameplayEffect> USpellHelperFunctionLibrary::GetEffectClass(FActiveGameplayEffectHandle Handle)
{
	const FActiveGameplayEffect* Effect = Handle.GetOwningAbilitySystemComponent()->GetActiveGameplayEffect(Handle);
	return Effect->Spec.Def.GetClass();
}

float USpellHelperFunctionLibrary::GetEffectSpecDuration(FGameplayEffectSpecHandle Handle)
{
	FGameplayEffectSpec* Spec = Handle.Data.Get();
	if (Spec)
	{
		return Spec->GetDuration();
	}
	return 0;
}
