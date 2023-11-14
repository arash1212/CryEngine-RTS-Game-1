#include "StdAfx.h"
#include "MoneyResource.h"
#include "GamePlugin.h"

int32 MoneyResource::GetPrice() const
{
	return nPrice;
}

string MoneyResource::GetName() const
{
	return sName;
}

string MoneyResource::GetIcon() const
{
	return sIcon;
}

string MoneyResource::GetBuyIcon() const
{
	return sBuyIcon;
}

string MoneyResource::GetSellIcon() const
{
	return sSellIcon;
}

string MoneyResource::GetAttachmentName() const
{
	return sAttachmentName;
}

EResourceType MoneyResource::GetType() const
{
	return mResourceType;
}