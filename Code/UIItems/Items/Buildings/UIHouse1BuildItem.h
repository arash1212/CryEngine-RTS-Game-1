#pragma once

#include <UIItems/IBaseUIItem.h>

class UIHouse1BuildItem : public IBaseUIItem {
public:
	UIHouse1BuildItem(IEntity* entity);

protected:
	IEntity* m_pEntity = nullptr;

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
};
