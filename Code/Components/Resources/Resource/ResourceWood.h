#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Physics/BoxPrimitiveComponent.h>

#include <Components/Managers/ResourceManager.h>
#include <Components/Resources/Resource.h>

class ResourceWoodComponent final : public IEntityComponent
{

public:
	ResourceWoodComponent() = default;
	virtual ~ResourceWoodComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<ResourceWoodComponent>& desc)
	{
		desc.SetGUID("{67BAE116-7A99-4323-B257-339303AAC59F}"_cry_guid);
		desc.SetEditorCategory("Resource");
	}

private:
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	Cry::DefaultComponents::CBoxPrimitiveComponent* m_pBboxComponent = nullptr;

	ResourceComponent* m_pResourceComponent = nullptr;

public:

};
