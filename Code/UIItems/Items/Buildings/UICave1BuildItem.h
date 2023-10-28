#pragma once

#include <UIItems/IBaseUIItem.h>

class UICave1BuildItem : public IBaseUIItem {
public:
	UICave1BuildItem(IEntity* entity);

protected:
	IEntity* m_pEntity = nullptr;

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
};


