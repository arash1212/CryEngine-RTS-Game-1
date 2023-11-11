#include "StdAfx.h"
#include "MoneyResource.h"
#include "GamePlugin.h"

int32 MoneyResource::GetPrice()
{
	return nPrice;
}

string MoneyResource::GetName()
{
	return sName;
}

string MoneyResource::GetIcon()
{
	return sIcon;
}

string MoneyResource::GetBuyIcon()
{
	return sBuyIcon;
}

string MoneyResource::GetSellIcon()
{
	return sSellIcon;
}

string MoneyResource::GetAttachmentName()
{
	return sAttachmentName;
}

EResourceType MoneyResource::GetType()
{
	return mResourceType;
}