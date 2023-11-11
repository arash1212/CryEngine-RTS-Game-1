#pragma once

#include <UIItems/IBaseUIItem.h>

class UIAlchemy1BuildItem : public IBaseUIItem {
public:
	UIAlchemy1BuildItem(IEntity* entity);

protected:
	IEntity* m_pEntity = nullptr;

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
	virtual SDescription GetDescrption() override;
};


