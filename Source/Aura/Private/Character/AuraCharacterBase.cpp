// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacterBase.h"

#include "GAS/AuraGameplayAbility.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	if (!HasAuthority()) return;

	for(TSubclassOf<UGameplayAbility> Ability : StartAbilities)
	{
		const FGameplayAbilitySpec Spec = FGameplayAbilitySpec(Ability, 1);
		AbilitySystemComponent->GiveAbility(Spec);
	}
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	check(IsValid(AbilitySystemComponent));
	if(InitializeEffect)
	{
		const FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
		const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(InitializeEffect, 1, ContextHandle);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void AAuraCharacterBase::ActivateAbility(FGameplayAbilitySpecHandle AbilitySpecHandle)
{
	AbilitySystemComponent->TryActivateAbility(AbilitySpecHandle);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AAuraCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

