#pragma once

#include <Components/ResourcePoints/BaseResourcePoint.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <Components/Managers/ResourceManager.h>

class IronResourcePointComponent final : public BaseResourcePointComponent
{

public:
	IronResourcePointComponent() = default;
	virtual ~IronResourcePointComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<IronResourcePointComponent>& desc)
	{
		desc.SetGUID("{E7D9A867-E0B8-44D3-9EDB-50DD2F2871B6}"_cry_guid);
		desc.SetEditorCategory("ResourcePoint");
		desc.AddBase< BaseResourcePointComponent>();
	}

private:

public:
};
