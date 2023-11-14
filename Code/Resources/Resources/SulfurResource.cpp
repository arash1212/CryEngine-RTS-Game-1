#include "StdAfx.h"
#include "SulfurResource.h"
#include "GamePlugin.h"

int32 SulfurResource::GetPrice() const
{
	return  nPrice;
}

string SulfurResource::GetName() const
{
	return sName;
}

string SulfurResource::GetIcon() const
{
	return sIcon;
}

string SulfurResource::GetBuyIcon() const
{
	return sBuyIcon;
}

string SulfurResource::GetSellIcon() const
{
	return sSellIcon;
}

string SulfurResource::GetAttachmentName() const
{
	return sAttachmentName;
}

EResourceType SulfurResource::GetType() const
{
	return mResourceType;
}
