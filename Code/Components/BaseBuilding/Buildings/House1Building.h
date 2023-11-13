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

static string HOUSE_BUILDING_1_MODEL_PATH = "Objects/buildings/house1/house1.cdf";
static string HOUSE_BUILDING_1_TRUSS_MODEL_PATH = "Objects/buildings/house1/truss/house1_truss.cgf";

class House1BuildingComponent final : public BuildingComponent
{

public:
	House1BuildingComponent() = default;
	virtual ~House1BuildingComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<House1BuildingComponent>& desc)
	{
		desc.SetGUID("{423D5D0D-5FBA-4E26-9FBD-F8F7CE77F4E7}"_cry_guid);
		desc.SetEditorCategory("Building");
		desc.AddBase<BuildingComponent>();
	}

private:

public:
	static SDescription GetDescription();
};
