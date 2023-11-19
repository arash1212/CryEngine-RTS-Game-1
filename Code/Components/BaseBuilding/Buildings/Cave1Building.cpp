#include "StdAfx.h"
#include "Cave1Building.h"
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
#include <Actions/Units/UnitWanderingRandomlyAction.h>
#include <Actions/Units/UnitAttackEnemyBaseAction.h>
#include <Actions/Buildings/BuildingSpawnZombiesAction.h>

#include <Components/BaseBuilding/Building.h>
#include <Utils/MathUtils.h>
#include <Components/Selectables/Cost.h>
#include <Components/Selectables/Attacker.h>

#include <Components/Managers/ResourceManager.h>

#include <Components/Selectables/Health.h>

#include <Utils/MathUtils.h>
#include <Utils/EntityUtils.h>
#include <Components/Controller/AIController.h>
#include <Components/Info/OwnerInfo.h>
#include <Components/Selectables/Units/Zombie1Unit.h>
#include <Components/Managers/ResourceManager.h>
#include <Components/Managers/ActionManager.h>

#include <Components/Effects/BulletTracer.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


namespace
{
	static void RegisterCave1BuildingComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Cave1BuildingComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterCave1BuildingComponent);
}


void Cave1BuildingComponent::Initialize()
{
	//AnimationComponent Initializations
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0, 0, 0)));
	m_pAnimationComponent->SetCharacterFile(CAVE_BUILDING_1_MODEL_PATH);
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/cave1.adb");
	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Idle");
	m_pAnimationComponent->SetAnimationDrivenMotion(false);
	m_pAnimationComponent->LoadFromDisk();
	m_pAnimationComponent->ResetCharacter();

	//BoxComponent Initialization
	m_pBboxComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CBoxPrimitiveComponent>();
	m_pBboxComponent->m_size = Vec3(3.f, 2.9f, 1.8f);
	m_pBboxComponent->m_bReactToCollisions = true;

	//DecalComponent(Placement) Initialization
	m_pDecalComponent = m_pEntity->CreateComponent<Cry::DefaultComponents::CDecalComponent>();
	m_pDecalComponent->SetTransformMatrix(Matrix34::Create(Vec3(3.6f, 4.2f, 3), IDENTITY, Vec3(0.0f, -0.1f, 0)));
	m_pDecalComponent->SetMaterialFileName(BUILDING_PLACEMENT_GREEN_DECAL_MATERIAL);
	m_pDecalComponent->SetSortPriority(50);
	m_pDecalComponent->SetDepth(10);
	m_pDecalComponent->Spawn();

	//BuildingComponent initialization
	SetPathToTrussMesh(CAVE_BUILDING_1_TRUSS_MODEL_PATH);
	SetIsHouse(false);
	SetImagePath(Cave1BuildingComponent::GetDescription().sIcon);

	SBuildingInfo buildingInfo;
	buildingInfo.m_populationProduces = 10;
	SetBuildingInfo(buildingInfo);
	//UIItems

	//Update bounding box
	AABB aabb;
	m_pEntity->GetLocalBounds(aabb);
	Vec3 min = Vec3(aabb.min.x - 2.f, aabb.min.y - 2.1f, aabb.min.z - 2);
	Vec3 max = Vec3(aabb.max.x + 3.5f, aabb.max.y + 4.1f, aabb.max.z);
	AABB newAABB = AABB(min, max);
	m_pEntity->SetLocalBounds(newAABB, true);

	//OwnerComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetComponent<OwnerInfoComponent>();

	//CostComponent Initializations
	m_pCostComponent = m_pEntity->GetOrCreateComponent<CostComponent>();
	m_pCostComponent->SetCost(Cave1BuildingComponent::GetDescription().price);

	//ActionManager Initializations
	m_pActionManagerComponent = m_pEntity->GetOrCreateComponent<ActionManagerComponent>();
	m_pActionManagerComponent->SetIsBuilding(true);

	//SkinAttachment Initialization
	m_pSkinAttachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByIndex(0);

	//Materials Initializations
	m_pDefaultMaterial = m_pSkinAttachment->GetIAttachmentObject()->GetBaseMaterial();

	//ResourceManagerComponent initialization
	m_pResourceManagerComponent = m_pEntity->GetOrCreateComponent<ResourceManagerComponent>();
	m_pResourceManagerComponent->SetIsPlayer(false);
	m_pResourceManagerComponent->AddOwnedEntity(m_pEntity);

}


Cry::Entity::EventFlags Cave1BuildingComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void Cave1BuildingComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {
		//OwnerComponent Initialization
		m_pOwnerInfoComponent = m_pEntity->GetOrCreateComponent<OwnerInfoComponent>();
		m_pOwnerInfoComponent->SetCanBeTarget(true);
		m_pOwnerInfoComponent->SetPlayer(EPlayer::FERAL);
		m_pOwnerInfoComponent->SetTeam(EPlayerTeam::FERAL);
		m_pOwnerInfoComponent->SetFaction(EPlayerFaction::FERAL);
		m_pOwnerInfoComponent->SetOwner(m_pEntity);

		//m_pBuildingComponent->SetMaxHealth(300.f);
		SetBuilt();

		bIsGameStarted = true;

	}break;
	case Cry::Entity::EEvent::Update: {
		f32 DeltaTime = event.fParam[0];

		//UpdateMaterial();
		//RotateSelectionDecal();
		CommandUnitsToAttack();

		//Timers
		if (fAttackTimePassed < fTimeBetweenAttacks) {
			fAttackTimePassed += 0.5f * DeltaTime;
		}

		//Add action for spawning zombies
		if (bIsGameStarted && !bSpawnZombiesActionAdded) {
			ActionManagerComponent* pActionManagerComponent = m_pEntity->GetComponent<ActionManagerComponent>();
			if (!pActionManagerComponent) {
				return;
			}
			pActionManagerComponent->AddAction(new BuildingSpawnZombiesAction(m_pEntity, 50));
			bSpawnZombiesActionAdded = true;

			//Find Hostiles
			m_hostilePlayers = EntityUtils::FindHostilePlayers(m_pEntity);
			bIsCheckedForHostiles = true;
		}

	}break;
	case Cry::Entity::EEvent::Reset: {
		bSpawnZombiesActionAdded = false;

	}break;
	default:
		break;
	}
}

void Cave1BuildingComponent::CommandUnitsToAttack()
{
	if (!bIsCheckedForHostiles) {
		return;
	}

	if (fAttackTimePassed >= fTimeBetweenAttacks) {
		if (m_pResourceManagerComponent->GetOwnedEntities().size() >= 10) {
			int32 unitsCommanded = 0;
			for (IEntity* entity : m_pResourceManagerComponent->GetOwnedEntities()) {
				AttackerComponent* pAttackerComponent = entity->GetComponent<AttackerComponent>();
				if (!pAttackerComponent) {
					continue;
				}

				unitsCommanded++;
				ActionManagerComponent* pActionManagerComponent = entity->GetComponent<ActionManagerComponent>();
				pActionManagerComponent->AddAction(new UnitAttackEnemyBaseAction(entity, m_hostilePlayers[0]));

				if (unitsCommanded >= 10) {
					break;
				}
			}
			fAttackTimePassed = 0;
		}
	}
}

SDescription Cave1BuildingComponent::GetDescription()
{
	SResourceInfo price;
	price.m_moneyAmount = 80;
	price.m_oilAmount = 30;
	price.m_populationAmount = 0;
	price.m_woodAmount = 50;

	SDescription m_pDescription;
	m_pDescription.sName = "Cave 1";
	m_pDescription.sDescription = "Cave 1 Building.";
	m_pDescription.sBuyDescription = "Build Cave 1 Building.";
	m_pDescription.price = price;
	m_pDescription.sIcon = "cave_1_icon.png";

	return m_pDescription;
}
