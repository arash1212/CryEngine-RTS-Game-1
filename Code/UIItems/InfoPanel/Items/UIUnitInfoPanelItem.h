#pragma once

#include <UIItems/InfoPanel/IBaseInfoPanelUIItem.h>

class UIUnitInfoPanelItem : public IBaseInfoPanelUIItem {
public:
	UIUnitInfoPanelItem(IEntity* entity, string imagePath);

protected:
	IEntity* m_pEntity = nullptr;
	string m_imagePath = "";

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
};
