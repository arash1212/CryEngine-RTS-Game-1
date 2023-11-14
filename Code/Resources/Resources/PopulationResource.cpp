#include "StdAfx.h"
#include "PopulationResource.h"
#include "GamePlugin.h"

int32 PopulationResource::GetPrice() const
{
	return nPrice;
}

string PopulationResource::GetName() const
{
	return sName;
}

string PopulationResource::GetIcon() const
{
	return sIcon;
}

string PopulationResource::GetBuyIcon() const
{
	return sBuyIcon;
}

string PopulationResource::GetSellIcon() const
{
	return sSellIcon;
}

string PopulationResource::GetAttachmentName() const
{
	return sAttachmentName;
}

EResourceType PopulationResource::GetType() const
{
	return mResourceType;
}
