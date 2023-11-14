#include "StdAfx.h"
#include "OilResource.h"
#include "GamePlugin.h"

int32 OilResource::GetPrice() const
{
	return nPrice;
}

string OilResource::GetName() const
{
	return sName;
}

string OilResource::GetIcon() const
{
	return sIcon;
}

string OilResource::GetBuyIcon() const
{
	return sBuyIcon;
}

string OilResource::GetSellIcon() const
{
	return sSellIcon;
}

string OilResource::GetAttachmentName() const
{
	return sAttachmentName;
}

EResourceType OilResource::GetType() const
{
	return mResourceType;
}
