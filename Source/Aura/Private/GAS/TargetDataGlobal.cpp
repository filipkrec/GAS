// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/TargetDataGlobal.h"
#include "Kismet/GameplayStatics.h"

UTargetDataGlobal* UTargetDataGlobal::CreateTargetDataGlobal(UGameplayAbility* OwningAbility, TEnumAsByte<ETargetRule> TargetRule)
{
	UTargetDataGlobal* MyObj = NewAbilityTask<UTargetDataGlobal>(OwningAbility);
	MyObj->TargetRule = TargetRule;
	return MyObj;
}

void UTargetDataGlobal::Activate()
{
	const FGameplayAbilityActorInfo* Info = Ability->GetCurrentActorInfo();
	
	const bool bIsLocallyControlled = Info->IsLocallyControlled();

	if(bIsLocallyControlled)
	{
		SendData();
	}
	else
	{
		FGameplayAbilitySpecHandle Handle = GetAbilitySpecHandle();
		FPredictionKey Key = GetActivationPredictionKey();
		
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(Handle,Key).AddUObject(this, &UTargetDataGlobal::OnTargetDataReplicatedCallback);

		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(Handle,Key);
		if(!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataGlobal::SendData()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	const FGameplayAbilityActorInfo* Info = Ability->GetCurrentActorInfo();
	
	const AActor* SourceActor = Info->AvatarActor.Get();
	const FVector SourceLocation = SourceActor->GetActorLocation();

	TArray<AActor*> AllActors;
	TArray<TWeakObjectPtr<AActor>> toReturn;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAuraCharacterBase::StaticClass(), AllActors);

	switch(TargetRule.GetValue())
	{
	case ETargetRule::Closest:
		{
			float ClosestDistance = TNumericLimits<float>::Max();
			AActor* Closest = nullptr;
			for (AActor* Actor : AllActors)
			{
				if(Actor == SourceActor)
				{
					continue;
				}
				
				const float CurrentDistance = FVector::Distance(Actor->GetActorLocation(), SourceLocation);
			
				if(Closest == nullptr || CurrentDistance < ClosestDistance)
				{
					Closest = Actor;
					ClosestDistance = CurrentDistance;
				}
			}
			toReturn.Add(Closest);
		}
		break;
	case ETargetRule::Furthest:
		{
			float FurthestDistance = 0.f;
			AActor* Furthest = nullptr;
			for (AActor* Actor : AllActors)
			{
				if(Actor == SourceActor)
				{
					continue;
				}
				
				const float CurrentDistance = FVector::Distance(Actor->GetActorLocation(), SourceLocation);
			
				if(Furthest == nullptr || CurrentDistance > FurthestDistance)
				{
					Furthest = Actor;
					FurthestDistance = CurrentDistance;
				}
			}
			toReturn.Add(Furthest);
			break;
		}
	case ETargetRule::AllOthers:
		{
			for (AActor* Actor : AllActors)
			{
				if(Actor == SourceActor)
				{
					continue;
				}
				
				toReturn.Add(Actor);
			}
		}
		break;
	}
	
	FGameplayAbilityTargetData_ActorArray* Data = new FGameplayAbilityTargetData_ActorArray();
	Data->TargetActorArray = toReturn;
	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(Data);
	
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey
		);

	if(ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataGlobal::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& Data,
	FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(),GetActivationPredictionKey());
}
