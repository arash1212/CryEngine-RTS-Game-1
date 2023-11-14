#include "StdAfx.h"
#include "FlourResource.h"
#include "GamePlugin.h"

int32 FlourResource::GetPrice() const
{
	return nPrice;
}

string FlourResource::GetName() const
{
	return sName;
}

string FlourResource::GetIcon() const
{
	return sIcon;
}

string FlourResource::GetBuyIcon() const
{
	return sBuyIcon;
}

string FlourResource::GetSellIcon() const
{
	return sSellIcon;
}

string FlourResource::GetAttachmentName() const
{
	return sAttachmentName;
}

EResourceType FlourResource::GetType() const
{
	return mResourceType;
}
