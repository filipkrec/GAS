// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributesData.generated.h"

UENUM(BlueprintType)
enum class EAttribute
{
	HP,
	MaxHP,
	Mana,
	MaxMana
};

USTRUCT(BlueprintType)
struct FAttributeData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	EAttribute Attribute = EAttribute::HP;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag Tag = FGameplayTag();
};

UCLASS()
class AURA_API UAttributesData : public UDataAsset
{
	GENERATED_BODY()
public:
	EAttribute GetAttribute(const FGameplayTag& Tag);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FAttributeData> AttributeData;
};
