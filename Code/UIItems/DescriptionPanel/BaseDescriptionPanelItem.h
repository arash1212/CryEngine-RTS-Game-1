#pragma once

#include <Resources/IResource.h>
#include <Components/UI/UIDescriptionsPanel.h>
#include <Components/Managers/ResourceManager.h>

class BaseDescriptionPanelItem {
public:
	BaseDescriptionPanelItem(const IResource* resource, int32 price, ResourceManagerComponent* resourceManagerComponent);
protected:
	UIDescriptionsPanelComponent* m_pUIDescriptionsPanelComponent = nullptr;
	ResourceManagerComponent* m_pResourceManagerComponent = nullptr;
	const IResource* m_pResource = nullptr;
	int32 nPrice;
	string sText = 0;
	int32 nIndex = -1;
	bool bIsRed = false;

public:
	void SetUIDescriptionsPanelComponent(UIDescriptionsPanelComponent* uIDescriptionsPanelComponent);
	const IResource* GetResource();

	void UpdateText();
	string GetText();

	void SetIndex(int32 index);
	int32 GetIndex();

	bool IsRed();
};