#pragma once

#include <UIItems/InfoPanel/IBaseInfoPanelUIItem.h>

class IUIInfoPanelItemProvider {
public:
	virtual IBaseInfoPanelUIItem* GetInfoPanelItem() = 0;
};