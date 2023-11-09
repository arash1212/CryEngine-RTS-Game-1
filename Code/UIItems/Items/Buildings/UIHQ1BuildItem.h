#pragma once

#include <UIItems/IBaseUIItem.h>

class UIHQ1BuildItem : public IBaseUIItem {
public:
	UIHQ1BuildItem(IEntity* entity);

protected:
	IEntity* m_pEntity = nullptr;

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
	virtual SUIItemDescription GetDescrption() override;
};