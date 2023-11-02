#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

//boilerplate, allows use of ATTRIBUTE_ACCESSORS
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

//Contains a set of attributes. Each attribute has a current value and base value which can be modified through GAS.
//Several different ability system components can use the same set of attributes (each in their own instance)
//Replication is handled internally by GAS, so modifying them through GAS will automatically replicate
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UAuraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Boilerplate for any attribute, this UPROPERTY, and OnRep UFUNCTION, + DOREPLIFETIME_CONDITION_NOTIFY in constructor
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes")
	FGameplayAttributeData Health;

	//Attributes retain base and current value only. Current value = base  value + buffs.
	//We keep MaxHealth separate from Health because it can be affected by changes separate from (current)Health changes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Attributes")
	FGameplayAttributeData Mana;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Attributes")
	FGameplayAttributeData MaxMana;

	
	//Helper function to access the property but through GAS (maintains replication) 
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);
	
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);
	
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);
	
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& _oldHealth) const;
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& _oldMaxHealth) const;
	
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& _oldMana) const;
	
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& _oldMaxMana) const;
};
