// Brett Meirhofer 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Components/ActorComponent.h"
#include "SpellComponent.generated.h"


class UGameplayEffect;
class UElement;
enum class ESpellStat : uint8;
struct FGameplayTag;
class USpell;

// Lets us toss team logic/spell logic onto arbitrary actors instead of having everything be children of some ASpellActor base class
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPELLBUILDER_API USpellComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USpellComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<USpell*> SpellList = {};

	// Premade spells made avaliable on ABP. Saves a huge amount of testing time
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<USpell>> DefaultSpells = {};

	// TODO: Implement this
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int> SpellHotBar = {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int SelectedSpell = 0;

	// Used for assigning xp and handling AI aggro
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName Team = "player";

	// Stores effect handles so we can use them whenever the attached actor needs to apply damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FGameplayEffectSpecHandle> EffectHandles;

	// Stores stats like damage/speed/manacost
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<ESpellStat, float> Stats;

	// Used for modifying particle colors
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UElement> DefaultElement;

	// Might not be valid. Always do a isvalid check after using this
	UFUNCTION(BlueprintCallable)
		USpell* GetCurrentSpell();

	// Might not be valid. Always do a isvalid check after using this
	UFUNCTION(BlueprintCallable)
		TSubclassOf<UGameplayAbility> GetCurrentAbility();
};


// Kitchen sink function library
UCLASS()
class SPELLBUILDER_API USpellHelperFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Not sure why the base c++ function here isn't exposed to BP
	// Useful for configuring the spellcomponent on actors before we finish spawning them
	UFUNCTION(BlueprintCallable)
		static AActor* BP_SpawnActorDeferred(AActor* Context, TSubclassOf<AActor> Class, FTransform Transform,
			AActor* Owner, APawn* Instigator, ESpawnActorCollisionHandlingMethod Collision, ESpawnActorScaleMethod ScaleMethod);

	UFUNCTION(BlueprintCallable)
	static void FinishSpawningActor(AActor* Actor, FTransform Transform);

	// Gets only enemies
	UFUNCTION(BlueprintCallable)
		static TArray<AActor*> FilterByEnemyTeam(const TArray<AActor*>& Array, FName Team);

	// Gets only allies
	UFUNCTION(BlueprintCallable)
		static TArray<AActor*> FilterByAllyTeam(const TArray<AActor*>& Array, FName Team);

	UFUNCTION(BlueprintCallable)
		static TArray<AActor*> SortByDistance(TArray<AActor*> Array, AActor* Target);

	// Handles collision tracing for both our beams and bolts.
	// TODO: Implement Bouncing and Homing
	UFUNCTION(BlueprintCallable)
		static void BeamTrace(AActor* Context, FTransform CamTransform, FVector SourceLoc, float Length, int Bounces, float SeekDistance, int Pierces, TArray<FVector>& Locs, TArray<AActor*>& Actors, const TArray<AActor*>& Ignore);


	// These might go into 
	UFUNCTION(BlueprintCallable)
		static bool IsActiveEffectHandleValid(FActiveGameplayEffectHandle Handle);

	UFUNCTION(BlueprintCallable)
		static float GetTimeRemainingEffect(FActiveGameplayEffectHandle Handle, float WorldTime);

	UFUNCTION(BlueprintCallable)
		static TSubclassOf<UGameplayEffect> GetEffectClass(FActiveGameplayEffectHandle Handle);

	UFUNCTION(BlueprintCallable)
		static float GetEffectSpecDuration(FGameplayEffectSpecHandle Handle);

};
