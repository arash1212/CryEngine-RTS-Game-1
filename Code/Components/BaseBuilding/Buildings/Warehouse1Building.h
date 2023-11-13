#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Geometry/StaticMeshComponent.h>
#include <DefaultComponents/Physics/BoxPrimitiveComponent.h>
#include <DefaultComponents/Effects/DecalComponent.h>

class BuildingComponent;
class SelectableComponent;
class CostComponent;
class ResourceStorageComponent;

struct SResourceInfo;
struct SDescription;

static string WAREHOUSE_BUILDING_1_MODEL_PATH = "Objects/buildings/warehouse1/warehouse1.cdf";
static string WAREHOUSE_BUILDING_1_TRUSS_MODEL_PATH = "Objects/buildings/warehouse1/truss/warehouse1_truss.cgf";

class Warehouse1BuildingComponent final : public BuildingComponent
{

public:
	Warehouse1BuildingComponent() = default;
	virtual ~Warehouse1BuildingComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<Warehouse1BuildingComponent>& desc)
	{
		desc.SetGUID("{7F0FD288-E898-4AC7-9A4D-6C9520E7124E}"_cry_guid);
		desc.SetEditorCategory("Building");
		desc.AddBase<BuildingComponent>();
	}

private:
	ResourceStorageComponent* m_pResourceStorageComponent = nullptr;

private:

public:
	static SDescription GetDescription();
};