#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Geometry/StaticMeshComponent.h>
#include <DefaultComponents/Physics/BoxPrimitiveComponent.h>
#include <DefaultComponents/Effects/DecalComponent.h>
#include <Components/BaseBuilding/Building.h>

class SelectableComponent;
class CostComponent;
class GuardPostComponent;

struct SResourceInfo;
struct SDescription;

static string GUARD_TOWER_BUILDING_1_MODEL_PATH = "Objects/buildings/guardtower1/guardtower1.cdf";
static string GUARD_TOWER_BUILDING_1_TRUSS_MODEL_PATH = "Objects/buildings/guardtower1/truss/guardtower1_truss.cgf";

class GaurdTower1BuildingComponent final : public BuildingComponent
{

public:
	GaurdTower1BuildingComponent() = default;
	virtual ~GaurdTower1BuildingComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<GaurdTower1BuildingComponent>& desc)
	{
		desc.SetGUID("{1D4B98C4-6504-444C-8CA5-E657FA614540}"_cry_guid);
		desc.SetEditorCategory("Building");
		desc.AddBase<BuildingComponent>();
	}

private:
	CostComponent* m_pCostComponent = nullptr;
	GuardPostComponent* m_pGuardPostComponent = nullptr;

private:
	DynArray<IEntity*> m_pWalls;
	Vec3 m_lastCheckPos = ZERO;

private:
	void CheckForOtherTowers();

public:
	virtual void Place(Vec3 at) override;
	static SDescription GetDescription();
};
