#pragma once

#include <Interfaces/IUIInfoPanelItem.h>

class BaseUnitComponent : public IEntityComponent, public IUIInfoPanelItem
{

public:
	BaseUnitComponent() = default;
	virtual ~BaseUnitComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<BaseUnitComponent>& desc)
	{
		desc.SetGUID("{2FEFBD88-E5E2-483C-B29F-042D2D42078D}"_cry_guid);
		desc.SetEditorCategory("Units");
	}

private:
	virtual IBaseInfoPanelUIItem* GetInfoPanelItem() override;
};