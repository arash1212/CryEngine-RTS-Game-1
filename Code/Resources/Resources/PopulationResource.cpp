#include "StdAfx.h"
#include "PopulationResource.h"
#include "GamePlugin.h"

int32 PopulationResource::GetPrice()
{
	return nPrice;
}

string PopulationResource::GetName()
{
	return sName;
}

string PopulationResource::GetIcon()
{
	return sIcon;
}

string PopulationResource::GetBuyIcon()
{
	return sBuyIcon;
}

string PopulationResource::GetSellIcon()
{
	return sSellIcon;
}

string PopulationResource::GetAttachmentName()
{
	return sAttachmentName;
}

EResourceType PopulationResource::GetType()
{
	return mResourceType;
}
