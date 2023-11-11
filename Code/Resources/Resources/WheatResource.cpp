#include "StdAfx.h"
#include "WheatResource.h"
#include "GamePlugin.h"

int32 WheatResource::GetPrice()
{
	return nPrice;
}

string WheatResource::GetName()
{
	return sName;
}

string WheatResource::GetIcon()
{
	return sIcon;
}

string WheatResource::GetBuyIcon()
{
	return sBuyIcon;
}

string WheatResource::GetSellIcon()
{
	return sSellIcon;
}

string WheatResource::GetAttachmentName()
{
	return sAttachmentName;
}

EResourceType WheatResource::GetType()
{
	return mResourceType;
}
