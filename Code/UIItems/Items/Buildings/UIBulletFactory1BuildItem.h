#pragma once

#include <UIItems/IBaseUIItem.h>

class UIBulletFactory1BuildItem : public IBaseUIItem {
public:
	UIBulletFactory1BuildItem(IEntity* entity);

protected:
	IEntity* m_pEntity = nullptr;

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
};
