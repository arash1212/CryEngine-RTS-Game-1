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

static string BARRACKS_BUILDING_1_MODEL_PATH = "Objects/buildings/barracks1/barracks1.cdf";
static string BARRACKS_BUILDING_1_TRUSS_MODEL_PATH = "Objects/buildings/barracks1/truss/barracks1_truss.cgf";

class Barracks1BuildingComponent final : public BuildingComponent
{

public:
	Barracks1BuildingComponent() = default;
	virtual ~Barracks1BuildingComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<Barracks1BuildingComponent>& desc)
	{
		desc.SetGUID("{A1FAAEAA-4E21-48B7-848B-AE93C1FFBFD6}"_cry_guid);
		desc.SetEditorCategory("Building");
		desc.AddBase<BuildingComponent>();
	}

private:
	CostComponent* m_pCostComponent = nullptr;

private:

public:
	static SDescription GetDescription();
};