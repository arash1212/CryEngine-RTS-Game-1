#pragma once

#include <UIItems/IBaseUIItem.h>

class UIGuardTower1BuildItem : public IBaseUIItem {
public:
	UIGuardTower1BuildItem(IEntity* entity);

protected:
	IEntity* m_pEntity = nullptr;

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
	virtual SDescription GetDescrption() override;
};

