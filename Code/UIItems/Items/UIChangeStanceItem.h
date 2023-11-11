#pragma once

#include <UIItems/IBaseUIItem.h>

class UIChangeStanceItem : public IBaseUIItem {
public:
	UIChangeStanceItem(IEntity* entity);

protected:
	IEntity* m_pEntity = nullptr;

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
	virtual SDescription GetDescrption() override;
};