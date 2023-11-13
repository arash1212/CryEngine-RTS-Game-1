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
struct SDescription;

static string CAVE_BUILDING_1_MODEL_PATH = "Objects/buildings/cave1/cave1.cdf";
static string CAVE_BUILDING_1_TRUSS_MODEL_PATH = "Objects/buildings/house1/truss/house1_truss.cgf";

class Cave1BuildingComponent final : public BuildingComponent
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
		desc.AddBase<BuildingComponent>();
	}

private:
	ResourceManagerComponent* m_pResourceManagerComponent = nullptr;
	CostComponent* m_pCostComponent = nullptr;

private:
	bool bIsGameStarted = false;

	f32 m_timeBetweenAttacks = 400.f;
	f32 m_attackTimePassed = 0.f;

	DynArray<IEntity*> m_hostilePlayers;
	bool bIsCheckedForHostiles = false;

	bool bSpawnZombiesActionAdded = false;
private:
	void CommandUnitsToAttack();

public:
	static SDescription GetDescription();
};
