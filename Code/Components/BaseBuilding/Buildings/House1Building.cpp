#include "StdAfx.h"
#include "House1Building.h"
#include "GamePlugin.h"

#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Controller/AIController.h>
#include <Components/Weapons/BaseWeapon.h>
#include <Components/Selectables/UnitAnimation.h>
#include <Components/Managers/ActionManager.h>

#include <Components/Selectables/Selectable.h>
#include <UIItems/IBaseUIItem.h>
#include <UIItems/Items/UICancelItem.h>
#include <UIItems/Items/UIChangeStanceItem.h>
#include <UIItems/Items/Buildings/UIHQ1BuildItem.h>
#include<UIItems/Items/Buildings/TrainUnits/UITrainEngineer1Item.h>
#include<UIItems/Items/Buildings/TrainUnits/UITrainSoldier1Item.h>

#include <Components/BaseBuilding/Building.h>
#include <Utils/MathUtils.h>
#include <Components/Selectables/Cost.h>

#include <Components/Managers/ResourceManager.h>

#include <Components/Selectables/Health.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


namespace
{
	static void RegisterHouse1BuildingComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(House1BuildingComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterHouse1BuildingComponent);
}


void House1BuildingComponent::Initialize()
{
	//AnimationComponent Initializations
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0, 0, 0)));
	m_pAnimationComponent->SetCharacterFile(HOUSE_BUILDING_1_MODEL_PATH);
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/house1.adb");
	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Idle");
	m_pAnimationComponent->SetAnimationDrivenMotion(false);
	m_pAnimationComponent->LoadFromDisk();
	m_pAnimationComponent->ResetCharacter();

	//BoxComponent Initialization
	m_pBboxComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CBoxPrimitiveComponent>();
	m_pBboxComponent->m_size = Vec3(3.f, 2.9f, 1.3f);
	m_pBboxComponent->m_bReactToCollisions = true;

	//DecalComponent(Placement) Initialization
	m_pDecalComponent = m_pEntity->CreateComponent<Cry::DefaultComponents::CDecalComponent>();
	m_pDecalComponent->SetTransformMatrix(Matrix34::Create(Vec3(3.3f, 3.9f, 3), IDENTITY, Vec3(-0.2f, -1.0f, 0)));
	m_pDecalComponent->SetMaterialFileName(BUILDING_PLACEMENT_GREEN_DECAL_MATERIAL);
	m_pDecalComponent->SetSortPriority(50);
	m_pDecalComponent->SetDepth(10);
	m_pDecalComponent->Spawn();

	//BuildingComponent initialization
	m_pBuildingComponent = m_pEntity->GetOrCreateComponent<BuildingComponent>();
	m_pBuildingComponent->SetPathToTrussMesh(HOUSE_BUILDING_1_TRUSS_MODEL_PATH);
	m_pBuildingComponent->SetIsHouse(true);
	m_pBuildingComponent->SetMaxHealth(300.f);

	SBuildingInfo buildingInfo;
	buildingInfo.m_populationProduces = 10;
	m_pBuildingComponent->SetBuildingInfo(buildingInfo);
	//UIItems

	//Update bounding box
	AABB aabb;
	m_pEntity->GetLocalBounds(aabb);
	Vec3 min = Vec3(aabb.min.x - 2.1f, aabb.min.y - 2.7f, aabb.min.z - 2);
	Vec3 max = Vec3(aabb.max.x + 3.1f, aabb.max.y + 2.9f, aabb.max.z);
	AABB newAABB = AABB(min, max);
	m_pEntity->SetLocalBounds(newAABB, true);

	//CostComponent Initializations
	m_pCostComponent = m_pEntity->GetOrCreateComponent<CostComponent>();
	m_pCostComponent->SetCost(House1BuildingComponent::GetCost());
}


Cry::Entity::EventFlags House1BuildingComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void House1BuildingComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];



	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

SResourceInfo House1BuildingComponent::GetCost()
{
	SResourceInfo cost;
	cost.m_moneyAmount = 20;
	cost.m_populationAmount = 0;
	cost.m_woodAmount = 20;
	return cost;
}