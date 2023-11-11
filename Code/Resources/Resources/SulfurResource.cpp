#include "StdAfx.h"
#include "SulfurResource.h"
#include "GamePlugin.h"

string SulfurResource::GetName()
{
	return sName;
}

string SulfurResource::GetIcon()
{
	return sIcon;
}

string SulfurResource::GetBuyIcon()
{
	return sBuyIcon;
}

string SulfurResource::GetSellIcon()
{
	return sSellIcon;
}

string SulfurResource::GetAttachmentName()
{
	return sAttachmentName;
}

EResourceType SulfurResource::GetType()
{
	return mResourceType;
}
