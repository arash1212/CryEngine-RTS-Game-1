#pragma once

#include <UIItems/IBaseUIItem.h>

class UIWindmillBuildItem : public IBaseUIItem {
public:
	UIWindmillBuildItem(IEntity* entity);

protected:
	IEntity* m_pEntity = nullptr;

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
};

