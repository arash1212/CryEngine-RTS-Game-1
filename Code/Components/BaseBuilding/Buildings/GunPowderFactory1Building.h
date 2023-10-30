#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Geometry/StaticMeshComponent.h>
#include <DefaultComponents/Physics/BoxPrimitiveComponent.h>
#include <DefaultComponents/Effects/DecalComponent.h>
#include <DefaultComponents/Effects/ParticleComponent.h>

class BuildingComponent;
class SelectableComponent;
class CostComponent;
class WorkplaceComponent;

struct SResourceInfo;

static string GUNPOWDER_FACTORY_BUILDING_1_MODEL_PATH = "Objects/buildings/gunpowderfactory1/gunpowderfactory1.cdf";
static string GUNPOWDER_FACTORY_1_TRUSS_MODEL_PATH = "Objects/buildings/gunpowderfactory1/truss/gunpowderfactory1_truss.cgf";

class GunPowderFactory1BuildingComponent final : public IEntityComponent
{

public:
	GunPowderFactory1BuildingComponent() = default;
	virtual ~GunPowderFactory1BuildingComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<GunPowderFactory1BuildingComponent>& desc)
	{
		desc.SetGUID("{2E30457B-A5B7-4BF6-9347-04586D1DB8B2}"_cry_guid);
		desc.SetEditorCategory("Building");
	}


private:
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	Cry::DefaultComponents::CStaticMeshComponent* m_pTrussMeshComponent = nullptr;
	Cry::DefaultComponents::CBoxPrimitiveComponent* m_pBboxComponent = nullptr;
	Cry::DefaultComponents::CDecalComponent* m_pDecalComponent = nullptr;
	Cry::DefaultComponents::CParticleComponent* m_pParticleComponent = nullptr;

	SelectableComponent* m_pSelectableComponent = nullptr;
	BuildingComponent* m_pBuildingComponent = nullptr;
	CostComponent* m_pCostComponent = nullptr;
	WorkplaceComponent* m_pWorkplaceComponent = nullptr;

	IEntity* m_pWarehouseEntity = nullptr;
	IAttachment* m_pWorkPositionAttachment = nullptr;

	bool bIsCollectedSulfur = false;
	bool bIsCollectedWood = false;
	bool bIsTransferedSulfurToFactory = false;
	bool bIsTransferedWoodToFactory = false;
	bool bIsProducedGunPowder = false;

	f32 m_timeBetweenWorks = 6.f;
	f32 m_workTimePassed = 0.f;
private:
	IEntity* FindClosestWarehouse();
	void UpdateAssignedWorkers();

public:
	static SResourceInfo GetCost();
};

