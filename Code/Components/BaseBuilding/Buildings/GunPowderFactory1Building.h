#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Geometry/StaticMeshComponent.h>
#include <DefaultComponents/Physics/BoxPrimitiveComponent.h>
#include <DefaultComponents/Effects/DecalComponent.h>
#include <DefaultComponents/Effects/ParticleComponent.h>

class BuildingComponent;
class SelectableComponent;
class CostComponent;
class WorkplaceComponent;

struct SResourceInfo;
struct SDescription;

static string GUNPOWDER_FACTORY_BUILDING_1_MODEL_PATH = "Objects/buildings/gunpowderfactory1/gunpowderfactory1.cdf";
static string GUNPOWDER_FACTORY_1_TRUSS_MODEL_PATH = "Objects/buildings/gunpowderfactory1/truss/gunpowderfactory1_truss.cgf";

class GunPowderFactory1BuildingComponent final : public BuildingComponent
{

public:
	GunPowderFactory1BuildingComponent() = default;
	virtual ~GunPowderFactory1BuildingComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<GunPowderFactory1BuildingComponent>& desc)
	{
		desc.SetGUID("{2E30457B-A5B7-4BF6-9347-04586D1DB8B2}"_cry_guid);
		desc.SetEditorCategory("Building");
		desc.AddBase<BuildingComponent>();
	}


private:
	Cry::DefaultComponents::CParticleComponent* m_pParticleComponent = nullptr;
	WorkplaceComponent* m_pWorkplaceComponent = nullptr;

	IEntity* m_pWarehouseEntity = nullptr;
	IAttachment* m_pWorkPositionAttachment = nullptr;

	bool bIsCollectedSulfurAndTransferedToFactory = false;
	bool bIsCollectedWoodAndTransferedToFactory = false;
	bool bIsProducedGunPowder = false;

private:
	void UpdateAssignedWorkers();

public:
	static SDescription GetDescription();
};

