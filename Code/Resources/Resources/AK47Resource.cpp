#include "StdAfx.h"
#include "AK47Resource.h"
#include "GamePlugin.h"

int32 AK47Resource::GetPrice()
{
	return nPrice;
}

string AK47Resource::GetName()
{
	return sName;
}

string AK47Resource::GetIcon()
{
	return sIcon;
}

string AK47Resource::GetBuyIcon()
{
	return sBuyIcon;
}

string AK47Resource::GetSellIcon()
{
	return sSellIcon;
}

string AK47Resource::GetAttachmentName()
{
	return sAttachmentName;
}

EResourceType AK47Resource::GetType()
{
	return mResourceType;
}