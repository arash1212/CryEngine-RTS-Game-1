#pragma once

#include <UIItems/IBaseUIItem.h>

class OwnerInfoComponent;
class ResourceManagerComponent;

class UISellWoodItem : public IBaseUIItem {
public:
	UISellWoodItem(IEntity* entity);

protected:
	IEntity* m_pEntity = nullptr;
	OwnerInfoComponent* m_pOwnerInfoComponent = nullptr;

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
	virtual SDescription GetDescrption() override;
};
