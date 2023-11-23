// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"

#include "GAS/AuraAttributeSet.h"

AAuraEnemy::AAuraEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = Cast<UAuraAttributeSet>(CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet"));
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	AddCharacterAbilities();
	InitAbilityActorInfo();
}

void AAuraEnemy::InitAbilityActorInfo()
{
	(Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))->AbilityActorInfoSet();
	InitializeDefaultAttributes();
}

void AAuraEnemy::HighlightActor()
{
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(250);
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(250);
}

void AAuraEnemy::UnhighlightActor()
{
	Weapon->SetRenderCustomDepth(false);
	Weapon->SetCustomDepthStencilValue(0);
	GetMesh()->SetRenderCustomDepth(false);
	GetMesh()->SetCustomDepthStencilValue(0);
}