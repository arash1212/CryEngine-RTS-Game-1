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

static string ALCHEMY_BUILDING_1_MODEL_PATH = "Objects/buildings/alchemy1/alchemy1.cdf";
static string ALCHEMY_BUILDING_1_TRUSS_MODEL_PATH = "Objects/buildings/alchemy1/truss/alchemy1_truss.cgf";

class Alchemy1BuildingComponent final : public BuildingComponent
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
		desc.AddBase<BuildingComponent>();
	}

private:
	WorkplaceComponent* m_pWorkplaceComponent = nullptr;

	IEntity* m_pWarehouseEntity = nullptr;
	IAttachment* m_pWorkPositionAttachment = nullptr;

	bool bIsCollectedOilAndTransferedToAlchemy = false;
	bool bIsProducedSulfur = false;

private:
	void UpdateAssignedWorkers();

public:
	static SDescription GetDescription();
};
