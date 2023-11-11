#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Physics/BoxPrimitiveComponent.h>

#include <Components/Managers/ResourceManager.h>

class ResourcePointComponent;

class OilResourcePointComponent final : public IEntityComponent
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
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	Cry::DefaultComponents::CBoxPrimitiveComponent* m_pBboxComponent = nullptr;

	ResourcePointComponent* m_pResourcePointComponent = nullptr;

public:
};