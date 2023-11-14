#pragma once

#include <Resources/IResource.h>

class BaseDescriptionPanelItem {
public:
	BaseDescriptionPanelItem(const IResource* resource, string amount);
protected:
	const IResource* m_pResource = nullptr;
	string sAmount = 0;
public:
	const IResource* GetResource();
	string GetAmount();
};