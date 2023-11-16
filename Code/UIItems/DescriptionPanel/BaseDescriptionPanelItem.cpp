#include "StdAfx.h"
#include "BaseDescriptionPanelItem.h"
#include "GamePlugin.h"

BaseDescriptionPanelItem::BaseDescriptionPanelItem(const IResource* resource, int32 price, ResourceManagerComponent* resourceManager)
{
	this->m_pResource = resource;
	this->nPrice = price;
	this->m_pResourceManagerComponent = resourceManager;

	UpdateText();
}

void BaseDescriptionPanelItem::SetUIDescriptionsPanelComponent(UIDescriptionsPanelComponent* uIDescriptionsPanelComponent)
{
	this->m_pUIDescriptionsPanelComponent = uIDescriptionsPanelComponent;
}

const IResource* BaseDescriptionPanelItem::GetResource()
{
	return m_pResource;
}

void BaseDescriptionPanelItem::UpdateText()
{
	SResourceRequestParams params = m_pResourceManagerComponent->GetResourceInfo(m_pResource->GetType());
	int32 nAvailableAmount = params.nAmount;
	int32 price = nPrice;
	if (price > nAvailableAmount) {
		bIsRed = true;
	}
	else {
		bIsRed = false;
	}

	string text = "";
	text.AppendFormat("%i (%i)", price, nAvailableAmount);
	this->sText = text;
}

string BaseDescriptionPanelItem::GetText()
{
	return sText;
}

void BaseDescriptionPanelItem::SetIndex(int32 index)
{
	this->nIndex = index;
}

int32 BaseDescriptionPanelItem::GetIndex()
{
	return nIndex;
}

bool BaseDescriptionPanelItem::IsRed()
{
	return bIsRed;
}

