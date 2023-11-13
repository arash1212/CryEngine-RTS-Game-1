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
struct SDescription;

static string FARM_BUILDING_1_MODEL_PATH = "Objects/buildings/farm1/farm1.cdf";
static string WHEAT_1_FARM_1_MODEL_PATH = "Objects/buildings/farm1/wheat/wheat1.cgf";
static string FARM_BUILDING_1_TRUSS_MODEL_PATH = "Objects/buildings/farm1/truss/farm1_truss.cgf";

class Farm1BuildingComponent final : public BuildingComponent
{

public:
	Farm1BuildingComponent() = default;
	virtual ~Farm1BuildingComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<Farm1BuildingComponent>& desc)
	{
		desc.SetGUID("{1E299424-AC01-4684-8EBF-E8E685CDDFA6}"_cry_guid);
		desc.SetEditorCategory("Building");
		desc.AddBase<BuildingComponent>();
	}


private:
	WorkplaceComponent* m_pWorkplaceComponent = nullptr;


	DynArray<IAttachment*> m_pWheatAttachments;
	DynArray<IAttachment*> m_pWheatSproutAttachments;
	IEntity* m_pWarehouseEntity = nullptr;

private:
	bool bIsPlantingDone = false;
	bool bIsHarvestingStarted = false;
	bool bIsHarvestingDone = false;
	Vec3 m_pWorker1CurrentAssignePos = ZERO;
	int32 m_currentIndex = 0;
	f32 m_timeBetweenWorks = 1.f;
	f32 m_workTimePassed = 0.f;

private:
	void UpdateAssignedWorkers();

public:
	static SDescription GetDescription();
};
