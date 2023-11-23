// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/MMC_Custom.h"

#include "GAS/AuraAttributeSet.h"

UMMC_Custom::UMMC_Custom()
{
	StatDef.AttributeToCapture = UAuraAttributeSet::GetMaxManaAttribute();
	StatDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	StatDef.bSnapshot = false;

	TargetStatDef.AttributeToCapture = UAuraAttributeSet::GetMaxManaAttribute();
	TargetStatDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	TargetStatDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(StatDef);
	RelevantAttributesToCapture.Add(TargetStatDef);
}

float UMMC_Custom::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Mana = 0.f;
	GetCapturedAttributeMagnitude(StatDef, Spec, EvaluationParameters, Mana);
	Mana = FMath::Max<float>(Mana, 0.f);

	
	float TargetMana = 0.f;
	GetCapturedAttributeMagnitude(TargetStatDef, Spec, EvaluationParameters, TargetMana);
	TargetMana = FMath::Max<float>(TargetMana, 0.f);

	return FMath::Max<float>(TargetMana - Mana, 0.f) * 2;
}