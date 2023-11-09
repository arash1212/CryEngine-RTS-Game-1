#pragma once

#include <UIItems/IBaseUIItem.h>

class UICancelItem : public IBaseUIItem {
public:
	UICancelItem(IEntity* entity);

protected :
	IEntity* m_pEntity = nullptr;

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
	virtual SUIItemDescription GetDescrption() override;
};