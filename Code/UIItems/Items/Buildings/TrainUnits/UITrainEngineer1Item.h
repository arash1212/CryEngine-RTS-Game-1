#pragma once

#include <UIItems/IBaseUIItem.h>

class UITrainEngineer1Item : public IBaseUIItem {
public:
	UITrainEngineer1Item(IEntity* entity);

protected:
	IEntity* m_pEntity = nullptr;

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
	virtual SUIItemDescription GetDescrption() override;
};