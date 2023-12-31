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

static string BAKERY_BUILDING_1_MODEL_PATH = "Objects/buildings/bakery1/bakery1.cdf";
static string BAKERY_BUILDING_1_TRUSS_MODEL_PATH = "Objects/buildings/bakery1/truss/bakery1_truss.cgf";

class Bakery1BuildingComponent final : public BuildingComponent
{

public:
	Bakery1BuildingComponent() = default;
	virtual ~Bakery1BuildingComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<Bakery1BuildingComponent>& desc)
	{
		desc.SetGUID("{6C50F53F-20BA-4CE0-BA6C-ED37F7D867EB}"_cry_guid);
		desc.SetEditorCategory("Building");
		desc.AddBase<BuildingComponent>();
	}

private:
	Cry::DefaultComponents::CParticleComponent* m_pParticleComponent = nullptr;
	WorkplaceComponent* m_pWorkplaceComponent = nullptr;

	IEntity* m_pWarehouseEntity = nullptr;
	IAttachment* m_pWorkPositionAttachment = nullptr;

private:
	bool bIsCollectedAndTransferedFlour = false;
	bool bIsProducedBread = false;

private:
	void UpdateAssignedWorkers();
	void ResetWorkersJob();

public:
	static SDescription GetDescription();
};
