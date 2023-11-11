#pragma once

#include <UIItems/IBaseUIItem.h>

class UIBarracks1BuildItem : public IBaseUIItem {
public:
	UIBarracks1BuildItem(IEntity* entity);

protected:
	IEntity* m_pEntity = nullptr;

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
	virtual SDescription GetDescrption() override;
};
