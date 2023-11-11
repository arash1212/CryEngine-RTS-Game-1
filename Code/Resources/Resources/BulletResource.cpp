#include "StdAfx.h"
#include "BulletResource.h"
#include "GamePlugin.h"

int32 BulletResource::GetPrice()
{
	return nPrice;
}

string BulletResource::GetName()
{
	return sName;
}

string BulletResource::GetIcon()
{
	return sIcon;
}

string BulletResource::GetBuyIcon()
{
	return sBuyIcon;
}

string BulletResource::GetSellIcon()
{
	return sSellIcon;
}

string BulletResource::GetAttachmentName()
{
	return sAttachmentName;
}

EResourceType BulletResource::GetType()
{
	return mResourceType;
}
