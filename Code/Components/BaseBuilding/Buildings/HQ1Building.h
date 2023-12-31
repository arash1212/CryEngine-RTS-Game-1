#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Geometry/StaticMeshComponent.h>
#include <DefaultComponents/Physics/BoxPrimitiveComponent.h>
#include <DefaultComponents/Effects/DecalComponent.h>

class BuildingComponent;
class SelectableComponent;
class CostComponent;

struct SResourceInfo;
struct SDescription;

static string HQ_BUILDING_1_MODEL_PATH = "Objects/buildings/hq1/hq1.cdf";
static string HQ_BUILDING_1_TRUSS_MODEL_PATH = "Objects/buildings/hq1/truss/hq1_truss.cgf";

class HQ1BuildingComponent final : public BuildingComponent
{

public:
	HQ1BuildingComponent() = default;
	virtual ~HQ1BuildingComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<HQ1BuildingComponent>& desc)
	{
		desc.SetGUID("{6DA19654-0B5C-4F78-A279-637EDF96EABE}"_cry_guid);
		desc.SetEditorCategory("Building");
		desc.AddBase<BuildingComponent>();
	}

private:

public:
	static SDescription GetDescription();
};