#pragma once

#include <UIItems/IBaseUIItem.h>

class UIWarehouse1BuildItem : public IBaseUIItem {
public:
	UIWarehouse1BuildItem(IEntity* entity);

protected:
	IEntity* m_pEntity = nullptr;

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
};
