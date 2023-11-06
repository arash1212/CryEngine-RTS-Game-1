#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Geometry/StaticMeshComponent.h>
#include <DefaultComponents/Physics/BoxPrimitiveComponent.h>
#include <DefaultComponents/Effects/DecalComponent.h>

class BuildingComponent;
class SelectableComponent;
class CostComponent;
class WorkplaceComponent;

struct SResourceInfo;

static string AK47_FACTORY_BUILDING_1_MODEL_PATH = "Objects/buildings/ak47factory1/ak47factory1.cdf";
static string AK47_FACTORY_1_TRUSS_MODEL_PATH = "Objects/buildings/gunpowderfactory1/truss/gunpowderfactory1_truss.cgf";

class AK47Factory1BuildingComponent final : public IEntityComponent
{

public:
	AK47Factory1BuildingComponent() = default;
	virtual ~AK47Factory1BuildingComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<AK47Factory1BuildingComponent>& desc)
	{
		desc.SetGUID("{2CA522D9-E6E1-49D6-80EE-272EE31434B9}"_cry_guid);
		desc.SetEditorCategory("Building");
	}


private:
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	Cry::DefaultComponents::CStaticMeshComponent* m_pTrussMeshComponent = nullptr;
	Cry::DefaultComponents::CBoxPrimitiveComponent* m_pBboxComponent = nullptr;
	Cry::DefaultComponents::CDecalComponent* m_pDecalComponent = nullptr;

	SelectableComponent* m_pSelectableComponent = nullptr;
	BuildingComponent* m_pBuildingComponent = nullptr;
	CostComponent* m_pCostComponent = nullptr;
	WorkplaceComponent* m_pWorkplaceComponent = nullptr;

	IEntity* m_pWarehouseEntity = nullptr;
	IAttachment* m_pWorkPositionAttachment = nullptr;

	bool bIsPickedupBulletAndTransferedTofactory = false;
	bool bIsPickedupIronAndTransferedTofactory = false;
	bool bIsProducedAK47 = false;

private:
	void UpdateAssignedWorkers();

public:
	static SResourceInfo GetCost();
};

