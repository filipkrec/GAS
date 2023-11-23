// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	OnMaxHealthChanged.Broadcast(AuraSet->GetMaxHealth());
	OnHealthChanged.Broadcast(AuraSet->GetHealth());
	OnManaChanged.Broadcast(AuraSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
	[this](const FGameplayTagContainer& AssetTags){
		for (const FGameplayTag& Tag : AssetTags)
		{
			FTagRow* Row = FTagRow::GetDataTableRow(MessageWidgetDataTable, Tag);
			if(Row)
			{
				const FUIWidgetRow* WidgetRow = static_cast<FUIWidgetRow*>(Row);
				OnMessageWidgetRow.Broadcast(*WidgetRow);
			}
		}
	}
	);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
	OnStatChanged.Broadcast();
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
	OnStatChanged.Broadcast();
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
	OnStatChanged.Broadcast();
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
	OnStatChanged.Broadcast();
}

FTagRow* FTagRow::GetDataTableRow(const UDataTable* DataTable, const FGameplayTag& Tag)
{
	TArray<FTagRow*> TableRows;
	DataTable->GetAllRows("", TableRows);
	
	for(FTagRow* Row : TableRows)
	{
		if(Row->MessageTag == Tag)
		{
			return Row;
		}
	}

	return nullptr;
}