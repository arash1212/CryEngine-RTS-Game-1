#include "StdAfx.h"
#include "BulletResource.h"
#include "GamePlugin.h"

int32 BulletResource::GetPrice() const
{
	return nPrice;
}

string BulletResource::GetName() const
{
	return sName;
}

string BulletResource::GetIcon() const
{
	return sIcon;
}

string BulletResource::GetBuyIcon() const
{
	return sBuyIcon;
}

string BulletResource::GetSellIcon() const
{
	return sSellIcon;
}

string BulletResource::GetAttachmentName() const
{
	return sAttachmentName;
}

EResourceType BulletResource::GetType() const
{
	return mResourceType;
}
