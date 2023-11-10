#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>

const string DEFAULT_VISIBILITY_INVISIBLE_MATERIAL = "Materials/visibility/m_visibility_invisible_1.mtl";

class VisibilityComponent final : public IEntityComponent
{

public:
	VisibilityComponent() = default;
	virtual ~VisibilityComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<VisibilityComponent>& desc)
	{
		desc.SetGUID("{BC998FF6-8B19-40BB-BD8A-99E0906F6BF6}"_cry_guid);
		desc.SetEditorCategory("Selectable");
	}

private:
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	IAttachmentManager* m_pAttachmentManager = nullptr;
	DynArray<IMaterial*> m_defaultMaterials;

	bool bIsVisible = true;
public:
	void SetVisible(bool isVisible);
	bool IsVisible();
};