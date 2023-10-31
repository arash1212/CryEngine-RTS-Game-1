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

static string BAKERY_BUILDING_1_MODEL_PATH = "Objects/buildings/bakery1/bakery1.cdf";
static string BAKERY_BUILDING_1_TRUSS_MODEL_PATH = "Objects/buildings/bakery1/truss/bakery1_truss.cgf";

class Bakery1BuildingComponent final : public IEntityComponent
{

public:
	Bakery1BuildingComponent() = default;
	virtual ~Bakery1BuildingComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<Bakery1BuildingComponent>& desc)
	{
		desc.SetGUID("{6C50F53F-20BA-4CE0-BA6C-ED37F7D867EB}"_cry_guid);
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

private:
	bool bIsCollectedFlour = false;
	bool bIsTransferedFlourToBakery = false;
	bool bIsProducedBread = false;

	f32 m_timeBetweenWorks = 5.f;
	f32 m_workTimePassed = 0.f;

private:
	void UpdateAssignedWorkers();

public:
	static SResourceInfo GetCost();
};
