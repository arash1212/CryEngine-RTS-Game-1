#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Physics/BoxPrimitiveComponent.h>

#include <Components/Managers/ResourceManager.h>
#include <Components/Resources/Resource.h>

class ResourceIronComponent final : public IEntityComponent
{

public:
	ResourceIronComponent() = default;
	virtual ~ResourceIronComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<ResourceIronComponent>& desc)
	{
		desc.SetGUID("{E7D9A867-E0B8-44D3-9EDB-50DD2F2871B6}"_cry_guid);
		desc.SetEditorCategory("Resource");
	}


private:
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	Cry::DefaultComponents::CBoxPrimitiveComponent* m_pBboxComponent = nullptr;

	ResourceComponent* m_pResourceComponent = nullptr;

public:
};
