#include "StdAfx.h"
#include "BreadResource.h"
#include "GamePlugin.h"

int32 BreadResource::GetPrice() const
{
	return nPrice;
}

string BreadResource::GetName() const
{
	return sName;
}

string BreadResource::GetIcon() const
{
	return sIcon;
}

string BreadResource::GetBuyIcon() const
{
	return sBuyIcon;
}

string BreadResource::GetSellIcon() const
{
	return sSellIcon;
}

string BreadResource::GetAttachmentName() const
{
	return sAttachmentName;
}

EResourceType BreadResource::GetType() const
{
	return mResourceType;
}