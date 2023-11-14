#pragma once

#include <UIItems/InfoPanel/IBaseInfoPanelUIItem.h>
#include <Interfaces/IUIInfoPanelItemProvider.h>

class IBaseAction :public IUIInfoPanelItemProvider {

protected:
	IEntity* m_pEntity = nullptr;
	bool bIsDone = false;

public:
	virtual void Execute() = 0;
	virtual void Cancel() = 0;
	virtual bool IsDone() = 0;
	virtual bool CanBeSkipped() {return false;}
	virtual f32 GetProgressAmount() { return 0; }
	virtual f32 GetMaxProgressAmount() { return 100.f; }
	virtual IBaseInfoPanelUIItem* GetInfoPanelItem() override { return nullptr; }
};