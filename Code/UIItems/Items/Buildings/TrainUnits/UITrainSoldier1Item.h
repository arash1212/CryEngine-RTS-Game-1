#pragma once

#include <UIItems/IBaseUIItem.h>

class UITrainSoldier1Item : public IBaseUIItem {
public:
	UITrainSoldier1Item(IEntity* entity);

protected:
	IEntity* m_pEntity = nullptr;

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
};
