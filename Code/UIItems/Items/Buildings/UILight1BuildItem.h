#pragma once

#include <UIItems/IBaseUIItem.h>

class UILight1BuildItem : public IBaseUIItem {
public:
	UILight1BuildItem(IEntity* entity);

protected:
	IEntity* m_pEntity = nullptr;

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
};

