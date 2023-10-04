#pragma once

#include "DefaultComponents/Effects/DecalComponent.h"

class SelectableComponent final : public IEntityComponent
{

public:
	SelectableComponent() = default;
	virtual ~SelectableComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<SelectableComponent>& desc)
	{
		desc.SetGUID("{1A76CDAA-E5DE-4397-BBCC-78E718A3EB4A}"_cry_guid);
		desc.SetEditorCategory("Units");
	}

private:
	Cry::DefaultComponents::CDecalComponent* m_pSelectionDecalComponent = nullptr;

private:
	bool bIsSelected = false;

public:
	void Select();
	void DeSelect();
	bool IsSelected();

};
