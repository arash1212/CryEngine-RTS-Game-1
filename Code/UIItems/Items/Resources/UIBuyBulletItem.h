#pragma once

#include <UIItems/IBaseUIItem.h>

class OwnerInfoComponent;
class ResourceManagerComponent;

class UIBuyBulletItem : public IBaseUIItem {
public:
	UIBuyBulletItem(IEntity* entity);

protected:
	IEntity* m_pEntity = nullptr;
	OwnerInfoComponent* m_pOwnerInfoComponent = nullptr;

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
};

