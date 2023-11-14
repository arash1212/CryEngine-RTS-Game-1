#include "StdAfx.h"
#include "WheatResource.h"
#include "GamePlugin.h"

int32 WheatResource::GetPrice() const
{
	return nPrice;
}

string WheatResource::GetName() const
{
	return sName;
}

string WheatResource::GetIcon() const
{
	return sIcon;
}

string WheatResource::GetBuyIcon() const
{
	return sBuyIcon;
}

string WheatResource::GetSellIcon() const
{
	return sSellIcon;
}

string WheatResource::GetAttachmentName() const
{
	return sAttachmentName;
}

EResourceType WheatResource::GetType() const
{
	return mResourceType;
}
