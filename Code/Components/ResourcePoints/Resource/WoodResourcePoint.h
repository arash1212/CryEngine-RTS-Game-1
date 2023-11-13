#pragma once

#include <Components/ResourcePoints/BaseResourcePoint.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <Components/Managers/ResourceManager.h>

class WoodResourcePointComponent final : public BaseResourcePointComponent
{

public:
	WoodResourcePointComponent() = default;
	virtual ~WoodResourcePointComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<WoodResourcePointComponent>& desc)
	{
		desc.SetGUID("{67BAE116-7A99-4323-B257-339303AAC59F}"_cry_guid);
		desc.SetEditorCategory("ResourcePoint");
	}

private:

public:

};
