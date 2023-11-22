#pragma once

#include <UIItems/IBaseUIItem.h>

class UIWall1BuildItem : public IBaseUIItem {
public:
	UIWall1BuildItem(IEntity* entity);

protected:
	IEntity* m_pEntity = nullptr;

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
	virtual SDescription GetDescrption() override;
};
