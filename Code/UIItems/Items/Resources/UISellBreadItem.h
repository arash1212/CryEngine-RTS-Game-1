#pragma once

#include <UIItems/IBaseUIItem.h>

class OwnerInfoComponent;
class ResourceManagerComponent;

class UISellBreadItem : public IBaseUIItem {
public:
	UISellBreadItem(IEntity* entity);

protected:
	IEntity* m_pEntity = nullptr;
	OwnerInfoComponent* m_pOwnerInfoComponent = nullptr;

public:
	virtual void Execute() override;
	virtual string GetImagePath() override;
};
