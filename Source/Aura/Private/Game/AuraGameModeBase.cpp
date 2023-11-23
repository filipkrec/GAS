// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AuraGameModeBase.h"

#include "AbilitySystemGlobals.h"

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
