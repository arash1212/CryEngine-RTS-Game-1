#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Geometry/StaticMeshComponent.h>
#include <DefaultComponents/Physics/BoxPrimitiveComponent.h>
#include <DefaultComponents/Effects/DecalComponent.h>

class BuildingComponent;
class SelectableComponent;
class CostComponent;
class Zombie1UnitComponent;
class OwnerInfoComponent;
class ResourceManagerComponent;

struct SResourceInfo;

static string CAVE_BUILDING_1_MODEL_PATH = "Objects/buildings/cave1/cave1.cdf";
static string CAVE_BUILDING_1_TRUSS_MODEL_PATH = "Objects/buildings/house1/truss/house1_truss.cgf";

class Cave1BuildingComponent final : public IEntityComponent
{

public:
	Cave1BuildingComponent() = default;
	virtual ~Cave1BuildingComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<Cave1BuildingComponent>& desc)
	{
		desc.SetGUID("{3395C71B-2CD0-4795-BF45-96F5B6C1784F}"_cry_guid);
		desc.SetEditorCategory("Building");
	}

private:
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	Cry::DefaultComponents::CStaticMeshComponent* m_pTrussMeshComponent = nullptr;
	Cry::DefaultComponents::CBoxPrimitiveComponent* m_pBboxComponent = nullptr;
	Cry::DefaultComponents::CDecalComponent* m_pDecalComponent = nullptr;

	OwnerInfoComponent* m_pOwnerInfoComponent = nullptr;
	ResourceManagerComponent* m_pResourceManagerComponent = nullptr;

	SelectableComponent* m_pSelectableComponent = nullptr;
	BuildingComponent* m_pBuildingComponent = nullptr;
	CostComponent* m_pCostComponent = nullptr;

private:
	bool bIsGameStarted = false;

	f32 m_timeBetweenSpawningZombies = 2.f;
	f32 m_spawnTimePassed = -0.f;
	int32 m_maxZombiesCount = 40;

	f32 m_timeBetweenAttacks = 60.f;
	f32 m_attackTimePassed = -0.f;

	DynArray<IEntity*> m_hostilePlayers;
	bool bIsCheckedForHostiles = false;
private:
	void ProcessSpawns();
	void CommandUnitsToAttack();

	void FindHostilePlayers();

public:
	static SResourceInfo GetCost();
};
