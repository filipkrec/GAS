// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AuraAttributeSet.h"

#include "GAS/AttributesData.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Health replicated, without condition (replication wise, eg only on owner = condition), always (even if set to same value)
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		if (NewValue > GetMaxHealth()) NewValue = GetMaxHealth();
		else if (NewValue < 0) NewValue = 0;
	}

	if (Attribute == GetManaAttribute())
	{
		if (NewValue > GetMaxMana()) NewValue = GetMaxMana();
		else if (NewValue < 0) NewValue = 0;
	}

	if (Attribute == GetMaxHealthAttribute())
	{
		if (NewValue < GetHealth())
		{
			SetHealth(NewValue);
		}
	}

	if (Attribute == GetMaxManaAttribute())
	{
		if (NewValue < GetMana())
		{
			SetMana(NewValue);
		}
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle source = Data.EffectSpec.GetEffectContext();
	UAbilitySystemComponent* SourceASC = source.GetOriginalInstigatorAbilitySystemComponent();
	if (IsValid(SourceASC) && IsValid(SourceASC->AbilityActorInfo->AvatarActor.Get()))
	{
		AActor* SourceActor = SourceASC->AbilityActorInfo->AvatarActor.Get();
		AController* SourcePC = SourceASC->AbilityActorInfo->PlayerController.Get();
		if (!SourcePC)
		{
			if (APawn* Pawn = Cast<APawn>(SourceActor))
			{
				SourcePC = Pawn->GetController();
			}
		}
	}

	AActor* TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& _oldHealth) const
{
	//GAS works with rollback. Client creates a change -> notifies the server of that change -> the server checks if the change is valid ->
	// if it is, applies value change, if not, rollbacks to _oldHealth
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, _oldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& _oldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, _oldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& _oldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, _oldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& _oldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, _oldMaxMana);
}

FGameplayAttribute UAuraAttributeSet::GetAttribute(UAttributesData* Data, FGameplayTag& Tag)
{
	EAttribute Attribute = Data->GetAttribute(Tag);
	
	switch(Attribute)
	{
	case(EAttribute::HP):
		return GetHealthAttribute();
	case(EAttribute::MaxHP):
		return GetMaxHealthAttribute();
	case(EAttribute::Mana):
		return GetManaAttribute();
	case(EAttribute::MaxMana):
		return GetMaxManaAttribute();
	}

	return GetHealthAttribute();
}

float UAuraAttributeSet::GetAttributeValue(UAttributesData* Data, FGameplayTag& Tag)
{
	return GetAttribute(Data, Tag).GetNumericValue(this);
}

TArray<FAttributeValue> UAuraAttributeSet::GetAttributeValues(UAttributesData* Data,TArray<FGameplayTag> Tags)
{
	TArray<FAttributeValue> ToReturn;
	for (FGameplayTag& Element : Tags)
	{
		ToReturn.Add(FAttributeValue(Element, GetAttributeValue(Data, Element)));
	}
	return ToReturn;
}

TArray<FAttributeValue> UAuraAttributeSet::GetAllAttributeValues(UAttributesData* Data)
{
	TArray<FAttributeValue> ToReturn;
	for (FAttributeData Element : Data->AttributeData)
	{
		ToReturn.Add(FAttributeValue(Element.Tag, GetAttributeValue(Data, Element.Tag)));
	}
	return ToReturn;
}
