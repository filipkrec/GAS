#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> TargetGameplayEffectClass)
{
	if(UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent((TargetActor)))
	{
		check(TargetGameplayEffectClass);
		FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
	
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(TargetGameplayEffectClass,ActorLevel, EffectContextHandle);
		UObject* source = EffectSpecHandle.Data.Get()->GetEffectContext().GetSourceObject();
		if (!source)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Source!"));
		}

		FActiveGameplayEffectHandle ActivEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

		const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
		if (bIsInfinite && GameplayEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			ActiveEffectHandles.Add(ActivEffectHandle, TargetASC);
		}
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (GameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, GameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (GameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, GameplayEffectClass);
	}

	if (GameplayEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent((TargetActor)))
		{
			TArray<FActiveGameplayEffectHandle> ToRemove;
			for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
			{
				if (TargetASC == HandlePair.Value)
				{
					TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
					ToRemove.Add(HandlePair.Key);
				}
			}
			for (FActiveGameplayEffectHandle Handle : ToRemove)
			{
				ActiveEffectHandles.FindAndRemoveChecked(Handle);
			}
		}
	}

	if (bDestroyOnRemoval)
	{
		Destroy();
	}
}

