#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Geometry/StaticMeshComponent.h>
#include <DefaultComponents/Physics/BoxPrimitiveComponent.h>
#include <DefaultComponents/Effects/DecalComponent.h>

class BuildingComponent;
class SelectableComponent;
class CostComponent;
class ResourceStorageComponent;
class WorkplaceComponent;

struct SResourceInfo;
struct SDescription;

static string WINDMILL_BUILDING_1_MODEL_PATH = "Objects/buildings/windmill/windmill.cdf";
static string WINDMILL_BUILDING_1_TRUSS_MODEL_PATH = "Objects/buildings/windmill/truss/windmill_truss.cgf";

class Windmill1BuildingComponent final : public BuildingComponent
{

public:
	Windmill1BuildingComponent() = default;
	virtual ~Windmill1BuildingComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<Windmill1BuildingComponent>& desc)
	{
		desc.SetGUID("{CC63C580-FBA7-4285-A38C-F3EEAC305717}"_cry_guid);
		desc.SetEditorCategory("Building");
		desc.AddBase<BuildingComponent>();
	}

private:
	WorkplaceComponent* m_pWorkplaceComponent = nullptr;

	IEntity* m_pWarehouseEntity = nullptr;
private:
	bool bIsCollectedWheatAndTransferedWheatToMill = false;
	bool bIsProducedFlour = false;

private:
	void UpdateAssignedWorkers();

public:
	static SDescription GetDescription();
};
