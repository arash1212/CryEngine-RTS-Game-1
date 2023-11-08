#pragma once

#include <UIItems/IBaseUIItem.h>

class IBaseInfoPanelUIItem : IBaseUIItem{

protected:
	IEntity* m_pEntity = nullptr;

public:
	virtual void Execute() = 0;
	virtual string GetImagePath() = 0;
};
