#pragma once

#include <Components/Managers/ResourceManager.h>

class CostComponent final : public IEntityComponent
{

public:
	CostComponent() = default;
	virtual ~CostComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CostComponent>& desc)
	{
		desc.SetGUID("{D28AE2FD-5E2C-422E-8B66-4D7223D1C186}"_cry_guid);
		desc.SetEditorCategory("Resource");
	}

private:
	SResourceInfo m_pCost;

public:
	void SetCost(SResourceInfo resource);
	SResourceInfo GetCost() const;
};