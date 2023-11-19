#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Physics/BoxPrimitiveComponent.h>
#include <Components/Managers/ResourceManager.h>
#include <Resources/IResource.h>

class SelectableComponent;

class BaseResourcePointComponent : public IEntityComponent
{

public:
	BaseResourcePointComponent() = default;
	virtual ~BaseResourcePointComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<BaseResourcePointComponent>& desc)
	{
		desc.SetGUID("{B5845C7E-9991-43CC-AF41-B38B8467131B}"_cry_guid);
		desc.SetEditorCategory("ResourcePoint");
	}

protected:
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	Cry::DefaultComponents::CBoxPrimitiveComponent* m_pBboxComponent = nullptr;
	SelectableComponent* m_pSelectableComponent = nullptr;
	IAttachment* m_pCollectingLocationAttachment = nullptr;

	EResourceType m_type = EResourceType::Money;

	bool bIsSingleUse = false;
	bool bIsInUse = false;

	IEntity* m_pCurrentCollector = nullptr;

	bool bHasCollectingLocation = false;
	DynArray<IEntity*> m_collectors;
public:

	Vec3 GetCollectingLocation();
	
	void SetType(EResourceType type);
	EResourceType GetType();

	void SetIsSingleUse(bool singleUse);
	bool IsSingleUse();

	bool IsInUse();
	void SetIsInUse(bool inUse);

	void SetCurrentCollector(IEntity* currentCollector);
	IEntity* GetCurrentCollector();

	void SetHasCollectingLocation(bool hasCollectingLocation);
	bool HasCollectingLocation();

	void AddCollector(IEntity* collectorEntity);
	void RemoveCollector(IEntity* collectorEntity);
	DynArray<IEntity*> GetCollectors();
};