#include "StdAfx.h"
#include "WoodResource.h"
#include "GamePlugin.h"

int32 WoodResource::GetPrice()
{
	return nPrice;
}

string WoodResource::GetName()
{
	return sName;
}

string WoodResource::GetIcon()
{
	return sIcon;
}

string WoodResource::GetBuyIcon()
{
	return sBuyIcon;
}

string WoodResource::GetSellIcon()
{
	return sSellIcon;
}

string WoodResource::GetAttachmentName()
{
	return sAttachmentName;
}

EResourceType WoodResource::GetType()
{
	return mResourceType;
}
