#include "StdAfx.h"
#include "OilResource.h"
#include "GamePlugin.h"

int32 OilResource::GetPrice()
{
	return nPrice;
}

string OilResource::GetName()
{
	return sName;
}

string OilResource::GetIcon()
{
	return sIcon;
}

string OilResource::GetBuyIcon()
{
	return sBuyIcon;
}

string OilResource::GetSellIcon()
{
	return sSellIcon;
}

string OilResource::GetAttachmentName()
{
	return sAttachmentName;
}

EResourceType OilResource::GetType()
{
	return mResourceType;
}
