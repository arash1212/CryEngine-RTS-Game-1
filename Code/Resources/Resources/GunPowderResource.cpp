#include "StdAfx.h"
#include "GunPowderResource.h"
#include "GamePlugin.h"

int32 GunPowderResource::GetPrice()
{
	return nPrice;
}

string GunPowderResource::GetName()
{
	return sName;
}

string GunPowderResource::GetIcon()
{
	return sIcon;
}

string GunPowderResource::GetBuyIcon()
{
	return sBuyIcon;
}

string GunPowderResource::GetSellIcon()
{
	return sSellIcon;
}

string GunPowderResource::GetAttachmentName()
{
	return sAttachmentName;
}

EResourceType GunPowderResource::GetType()
{
	return mResourceType;
}
