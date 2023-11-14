#include "StdAfx.h"
#include "BaseDescriptionPanelItem.h"
#include "GamePlugin.h"

BaseDescriptionPanelItem::BaseDescriptionPanelItem(const IResource* resource, string amount)
{
	this->m_pResource = resource;
	this->sAmount = amount;
}

const IResource* BaseDescriptionPanelItem::GetResource()
{
	return m_pResource;
}

string BaseDescriptionPanelItem::GetAmount()
{
	return sAmount;
}
