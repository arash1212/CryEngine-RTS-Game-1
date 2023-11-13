#pragma once

#include <UIItems/InfoPanel/IBaseInfoPanelUIItem.h>

class IUIInfoPanelItem {
public:
	virtual IBaseInfoPanelUIItem* GetInfoPanelItem() = 0;
};