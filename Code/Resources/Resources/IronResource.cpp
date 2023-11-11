#include "StdAfx.h"
#include "IronResource.h"
#include "GamePlugin.h"

int32 IronResource::GetPrice()
{
	return nPrice;
}

string IronResource::GetName()
{
	return sName;
}

string IronResource::GetIcon()
{
	return sIcon;
}

string IronResource::GetBuyIcon()
{
	return sBuyIcon;
}

string IronResource::GetSellIcon()
{
	return sSellIcon;
}

string IronResource::GetAttachmentName()
{
	return sAttachmentName;
}

EResourceType IronResource::GetType()
{
	return mResourceType;
}
