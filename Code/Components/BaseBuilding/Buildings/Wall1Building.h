#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Geometry/StaticMeshComponent.h>
#include <DefaultComponents/Physics/BoxPrimitiveComponent.h>
#include <DefaultComponents/Effects/DecalComponent.h>
#include <Components/BaseBuilding/Building.h>

class SelectableComponent;
class CostComponent;

struct SResourceInfo;
struct SDescription;

static string WALL_BUILDING_1_MODEL_PATH = "Objects/buildings/wall1/wall1.cdf";
static string WALL_BUILDING_1_TRUSS_MODEL_PATH = "Objects/buildings/wall1/truss/wall1_truss.cgf";

class Wall1BuildingComponent final : public BuildingComponent
{

public:
	Wall1BuildingComponent() = default;
	virtual ~Wall1BuildingComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<Wall1BuildingComponent>& desc)
	{
		desc.SetGUID("{378A03A5-9B7B-4150-A205-3FB2BB829F0B}"_cry_guid);
		desc.SetEditorCategory("Building");
		desc.AddBase<BuildingComponent>();
	}

private:
	CostComponent* m_pCostComponent = nullptr;

private:

public:
	static SDescription GetDescription();
};
