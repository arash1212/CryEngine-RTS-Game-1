#include "StdAfx.h"
#include "BaseDescriptionPanelItem.h"
#include "GamePlugin.h"

BaseDescriptionPanelItem::BaseDescriptionPanelItem(string imagePath, int32 amount)
{
	this->m_imagePath = imagePath;
	this->m_amount = amount;
}

string BaseDescriptionPanelItem::GetImagePath()
{
	return m_imagePath;
}

int32 BaseDescriptionPanelItem::GetAmount()
{
	return m_amount;
}
