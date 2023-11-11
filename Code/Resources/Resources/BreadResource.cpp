
#include "StdAfx.h"
#include "BreadResource.h"
#include "GamePlugin.h"

int32 BreadResource::GetPrice()
{
	return nPrice;
}

string BreadResource::GetName()
{
	return sName;
}

string BreadResource::GetIcon()
{
	return sIcon;
}

string BreadResource::GetBuyIcon()
{
	return sBuyIcon;
}

string BreadResource::GetSellIcon()
{
	return sSellIcon;
}

string BreadResource::GetAttachmentName()
{
	return sAttachmentName;
}

EResourceType BreadResource::GetType()
{
	return mResourceType;
}