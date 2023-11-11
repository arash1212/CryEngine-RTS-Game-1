#pragma once

#include <UIItems/IBaseUIItem.h>

class UIGunPowderFactory1BuildItem : public IBaseUIItem {
public:
	UIGunPowderFactory1BuildItem(IEntity* entity);

protected:
	IEntity* m_pEntity = nullptr;

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
	virtual SDescription GetDescrption() override;
};


