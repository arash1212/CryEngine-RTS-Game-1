#include "StdAfx.h"
#include "IronResource.h"
#include "GamePlugin.h"

int32 IronResource::GetPrice() const
{
	return nPrice;
}

string IronResource::GetName() const
{
	return sName;
}

string IronResource::GetIcon() const
{
	return sIcon;
}

string IronResource::GetBuyIcon() const
{
	return sBuyIcon;
}

string IronResource::GetSellIcon() const
{
	return sSellIcon;
}

string IronResource::GetAttachmentName() const
{
	return sAttachmentName;
}

EResourceType IronResource::GetType() const
{
	return mResourceType;
}
