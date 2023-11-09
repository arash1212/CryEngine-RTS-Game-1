#pragma once

#include <UIItems/IBaseUIItem.h>

class UIAK47Factory1BuildItem : public IBaseUIItem {
public:
	UIAK47Factory1BuildItem(IEntity* entity);

protected:
	IEntity* m_pEntity = nullptr;

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
	virtual SUIItemDescription GetDescrption() override;
};


