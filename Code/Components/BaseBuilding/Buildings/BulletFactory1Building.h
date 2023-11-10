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

static string BULLET_FACTORY_BUILDING_1_MODEL_PATH = "Objects/buildings/bulletfactory1/bulletfactory1.cdf";
static string BULLET_FACTORY_1_TRUSS_MODEL_PATH = "Objects/buildings/gunpowderfactory1/truss/gunpowderfactory1_truss.cgf";

class BulletFactory1BuildingComponent final : public IEntityComponent
{

public:
	BulletFactory1BuildingComponent() = default;
	virtual ~BulletFactory1BuildingComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<BulletFactory1BuildingComponent>& desc)
	{
		desc.SetGUID("{0FE985BB-1CAD-4FA4-94F3-3006025E71DE}"_cry_guid);
		desc.SetEditorCategory("Building");
	}


private:
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	Cry::DefaultComponents::CStaticMeshComponent* m_pTrussMeshComponent = nullptr;
	Cry::DefaultComponents::CBoxPrimitiveComponent* m_pBboxComponent = nullptr;
	Cry::DefaultComponents::CDecalComponent* m_pDecalComponent = nullptr;
	Cry::DefaultComponents::CParticleComponent* m_pParticleComponent = nullptr;

	SelectableComponent* m_pSelectableComponent = nullptr;
	BuildingComponent* m_pBuildingComponent = nullptr;
	CostComponent* m_pCostComponent = nullptr;
	WorkplaceComponent* m_pWorkplaceComponent = nullptr;

	IEntity* m_pWarehouseEntity = nullptr;
	IAttachment* m_pWorkPositionAttachment = nullptr;

	bool bIsCollectedGunPowderAndTransferedToFactory = false;
	bool bIsCollectedIronAndTransferedToFactory = false;
	bool bIsProducedBullet = false;
private:
	void UpdateAssignedWorkers();

public:
	static SDescription GetDescription();
};

