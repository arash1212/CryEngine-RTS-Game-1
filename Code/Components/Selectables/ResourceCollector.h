#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <Components/Resources/Resource.h>
#include <Components/Managers/ResourceManager.h>

class OwnerInfoComponent;

class ResourceCollectorComponent final : public IEntityComponent
{

public:
	ResourceCollectorComponent() = default;
	virtual ~ResourceCollectorComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<ResourceCollectorComponent>& desc)
	{
		desc.SetGUID("{0086DD74-7806-41D8-AA21-35AC3710B989}"_cry_guid);
		desc.SetEditorCategory("Resource");
	}

private :
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	//OwnerShip
	OwnerInfoComponent* m_pOwnerInfoComponent = nullptr;

	EResourceType m_pCurrentResourceType = EResourceType::OIL;

	//Atachments
	//Resource
	IAttachment* m_pOilBarrelAttachment = nullptr;
	IAttachment* m_pWheatAttachment = nullptr;
	IAttachment* m_pFlourAttachment = nullptr;
	IAttachment* m_pWoodAttachment = nullptr;
	IAttachment* m_pBreadAttachment = nullptr;
	IAttachment* m_pSulfurAttachment = nullptr;
	IAttachment* m_pGunPowderAttachment = nullptr;
	IAttachment* m_pIronAttachment = nullptr;
	IAttachment* m_pBulletAttachment = nullptr;
	IAttachment* m_pAK47Attachment = nullptr;

private:
	int32 m_amountResourceCollected = 0;
	int32 m_maxResouceCanBeCollected = 40;

private:
	void UpdateResourceAttachment();

public:
	void AddResource(int32 amount);
	int32 GetAmountOfResourceCollected();
	void SendResourceToWareHouse();

	int32 GetMaxResourceCanBeCollected();

	void SetCurrentResourceType(EResourceType resourceType);
	EResourceType GetCurrentResourceType();

	bool CanCollectResource();
	void EmptyResources();
};
