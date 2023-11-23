// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AuraGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"


void UAuraGameplayAbility::ApplyEffectToTargets(FGameplayAbilityTargetDataHandle TargetData)
{
	if(GameplayEffectClasses.Num() == 0) return;
	
	TArray<AActor*> Targets = UAbilitySystemBlueprintLibrary::GetAllActorsFromTargetData(TargetData);
	for (AActor* Target : Targets)
	{
		if(UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target))
		{
			FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
			EffectContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());

			for(const TSubclassOf<UGameplayEffect>& Effect : GameplayEffectClasses)
			{
				FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(Effect, GetAbilityLevel(), EffectContextHandle);
				TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
			}
		}
	}
}
