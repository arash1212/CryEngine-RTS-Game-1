#pragma once

#include <UIItems/InfoPanel/IBaseInfoPanelUIItem.h>
#include <Actions/IBaseAction.h>

class UIBuildingActionInfoPanelItem : public IBaseInfoPanelUIItem {
public:
	UIBuildingActionInfoPanelItem(IEntity* entity, string imagePath, IBaseAction* action);

protected:
	IEntity* m_pEntity = nullptr;
	string m_imagePath = "";
	IBaseAction* m_pAction = nullptr;

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
};

