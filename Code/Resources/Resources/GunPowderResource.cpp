#include "StdAfx.h"
#include "GunPowderResource.h"
#include "GamePlugin.h"

int32 GunPowderResource::GetPrice() const
{
	return nPrice;
}

string GunPowderResource::GetName() const
{
	return sName;
}

string GunPowderResource::GetIcon() const
{
	return sIcon;
}

string GunPowderResource::GetBuyIcon() const
{
	return sBuyIcon;
}

string GunPowderResource::GetSellIcon() const
{
	return sSellIcon;
}

string GunPowderResource::GetAttachmentName() const
{
	return sAttachmentName;
}

EResourceType GunPowderResource::GetType() const
{
	return mResourceType;
}
