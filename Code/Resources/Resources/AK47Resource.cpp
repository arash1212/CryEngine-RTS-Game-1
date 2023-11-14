#include "StdAfx.h"
#include "AK47Resource.h"
#include "GamePlugin.h"

int32 AK47Resource::GetPrice() const
{
	return nPrice;
}

string AK47Resource::GetName() const
{
	return sName;
}

string AK47Resource::GetIcon() const
{
	return sIcon;
}

string AK47Resource::GetBuyIcon() const
{
	return sBuyIcon;
}

string AK47Resource::GetSellIcon() const
{
	return sSellIcon;
}

string AK47Resource::GetAttachmentName() const
{
	return sAttachmentName;
}

EResourceType AK47Resource::GetType() const
{
	return mResourceType;
}