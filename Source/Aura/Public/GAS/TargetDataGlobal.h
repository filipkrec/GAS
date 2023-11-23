// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataGlobal.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetDataGlobalSignature, const FGameplayAbilityTargetDataHandle, Data);

UENUM(BlueprintType)
enum class ETargetRule
{
	AllOthers,
	Furthest,
	Closest
};

UCLASS()
class AURA_API UTargetDataGlobal : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Ability", meta=(HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = true))
	static UTargetDataGlobal* CreateTargetDataGlobal(UGameplayAbility* OwningAbility, TEnumAsByte<ETargetRule> TargetRule);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETargetRule> TargetRule;
	
private:
	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FTargetDataGlobalSignature ValidData;

	void SendData();
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& Data, FGameplayTag ActivationTag);
};
