#include "StdAfx.h"
#include "FlourResource.h"
#include "GamePlugin.h"

int32 FlourResource::GetPrice()
{
	return nPrice;
}

string FlourResource::GetName()
{
	return sName;
}

string FlourResource::GetIcon()
{
	return sIcon;
}

string FlourResource::GetBuyIcon()
{
	return sBuyIcon;
}

string FlourResource::GetSellIcon()
{
	return sSellIcon;
}

string FlourResource::GetAttachmentName()
{
	return sAttachmentName;
}

EResourceType FlourResource::GetType()
{
	return mResourceType;
}
