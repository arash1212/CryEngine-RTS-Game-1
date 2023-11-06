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

static string ALCHEMY_BUILDING_1_MODEL_PATH = "Objects/buildings/alchemy1/alchemy1.cdf";
static string ALCHEMY_BUILDING_1_TRUSS_MODEL_PATH = "Objects/buildings/alchemy1/truss/alchemy1_truss.cgf";

class Alchemy1BuildingComponent final : public IEntityComponent
{

public:
	Alchemy1BuildingComponent() = default;
	virtual ~Alchemy1BuildingComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<Alchemy1BuildingComponent>& desc)
	{
		desc.SetGUID("{4371C932-5D06-401B-B58E-CEFFD305BD11}"_cry_guid);
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

	bool bIsCollectedOilAndTransferedToAlchemy = false;
	bool bIsProducedSulfur = false;

private:
	void UpdateAssignedWorkers();

public:
	static SResourceInfo GetCost();
};