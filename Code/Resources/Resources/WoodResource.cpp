#include "StdAfx.h"
#include "WoodResource.h"
#include "GamePlugin.h"

int32 WoodResource::GetPrice() const
{
	return nPrice;
}

string WoodResource::GetName() const
{
	return sName;
}

string WoodResource::GetIcon() const
{
	return sIcon;
}

string WoodResource::GetBuyIcon() const
{
	return sBuyIcon;
}

string WoodResource::GetSellIcon() const
{
	return sSellIcon;
}

string WoodResource::GetAttachmentName() const
{
	return sAttachmentName;
}

EResourceType WoodResource::GetType() const
{
	return mResourceType;
}
