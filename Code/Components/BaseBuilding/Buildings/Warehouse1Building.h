#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Geometry/StaticMeshComponent.h>
#include <DefaultComponents/Physics/BoxPrimitiveComponent.h>

class BuildingComponent;
class SelectableComponent;
class CostComponent;
class ResourceStorageComponent;

struct SResourceInfo;

static string WAREHOUSE_BUILDING_1_MODEL_PATH = "Objects/buildings/warehouse1/warehouse1.cdf";
static string WAREHOUSE_BUILDING_1_TRUSS_MODEL_PATH = "Objects/buildings/hq1/truss/hq1_truss.cgf";

class Warehouse1BuildingComponent final : public IEntityComponent
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
	}



private:
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	Cry::DefaultComponents::CStaticMeshComponent* m_pTrussMeshComponent = nullptr;
	Cry::DefaultComponents::CBoxPrimitiveComponent* m_pBboxComponent = nullptr;

	SelectableComponent* m_pSelectableComponent = nullptr;
	BuildingComponent* m_pBuildingComponent = nullptr;
	CostComponent* m_pCostComponent = nullptr;
	ResourceStorageComponent* m_pResourceStorageComponent = nullptr;

private:

public:
	static SResourceInfo GetCost();
};