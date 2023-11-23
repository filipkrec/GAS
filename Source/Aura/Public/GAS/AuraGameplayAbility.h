// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTargets(FGameplayAbilityTargetDataHandle TargetData);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effects)
	TArray<TSubclassOf<UGameplayEffect>> GameplayEffectClasses;
};
