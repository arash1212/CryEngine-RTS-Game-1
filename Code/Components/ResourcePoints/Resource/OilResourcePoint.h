#pragma once

#include <Components/ResourcePoints/BaseResourcePoint.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <Components/Managers/ResourceManager.h>


class OilResourcePointComponent final : public BaseResourcePointComponent
{

public:
	OilResourcePointComponent() = default;
	virtual ~OilResourcePointComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<OilResourcePointComponent>& desc)
	{
		desc.SetGUID("{6B6643B8-7880-4829-80FD-F88A89EF6DCD}"_cry_guid);
		desc.SetEditorCategory("ResourcePoint");
	}

private:

public:
};