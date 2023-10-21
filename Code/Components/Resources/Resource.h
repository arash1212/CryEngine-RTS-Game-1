#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <Components/Managers/ResourceManager.h>

class SelectableComponent;

enum class EResourceType {
	Money,
	OIL,
	POPULATION,
	WHEAT
};

class ResourceComponent final : public IEntityComponent
{

public:
	ResourceComponent() = default;
	virtual ~ResourceComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<ResourceComponent>& desc)
	{
		desc.SetGUID("{B5845C7E-9991-43CC-AF41-B38B8467131B}"_cry_guid);
		desc.SetEditorCategory("Resource");
	}

private:
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	SelectableComponent* m_pSelectableComponent = nullptr;
	IAttachment* m_pCollectingLocationAttachment = nullptr;

	EResourceType m_type = EResourceType::Money;
public:

	Vec3 GetCollectingLocation();
	
	void SetType(EResourceType type);
	EResourceType GetType();
};