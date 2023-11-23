// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AttributesData.h"


EAttribute UAttributesData::GetAttribute(const FGameplayTag& Tag)
{
	for (FAttributeData Element : AttributeData)
	{
		if(Element.Tag == Tag)
		{
			return Element.Attribute;
		}
	}
	
	return EAttribute::HP;
}
	