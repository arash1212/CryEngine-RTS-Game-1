#include "StdAfx.h"
#include "Engineer1Unit.h"
#include "GamePlugin.h"

#include <Components/Selectables/Attacker.h>

#include <Components/Selectables/Selectable.h>
#include <UIItems/IBaseUIItem.h>
#include <UIItems/Items/UICancelItem.h>
#include <UIItems/Items/UIChangeStanceItem.h>
#include <UIItems/Items/Buildings/UIHQ1BuildItem.h>
#include <UIItems/Items/Buildings/UIBarracks1BuildItem.h>
#include <UIItems/Items/Buildings/UIWarehouse1BuildItem.h>
#include <UIItems/Items/Buildings/UIHouse1BuildItem.h>
#include <UIItems/Items/Buildings/UIFarm1BuildItem.h>
#include <UIItems/Items/Buildings/UIWindmillBuildItem.h>
#include <UIItems/Items/Buildings/UIBakery1BuildItem.h>
#include <UIItems/Items/Buildings/UILight1BuildItem.h>
#include <UIItems/Items/Buildings/UIAlchemy1BuildItem.h>
#include <UIItems/Items/Buildings/UIGunPowderFactory1BuildItem.h>
#include <UIItems/Items/Buildings/UIBulletFactory1BuildItem.h>
#include <UIItems/Items/Buildings/UIAK47Factory1BuildItem.h>

#include <Components/Selectables/Worker.h>

#include <Components/Controller/AIController.h>
#include <Components/Managers/ActionManager.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Selectables/Engineer.h>

#include <CryAnimation/ICryAnimation.h>
#include <Components/Weapons/BaseWeapon.h>

#include <Components/Info/OwnerInfo.h>

#include <Components/Selectables/UnitAnimation.h>
#include <Components/Selectables/Cost.h>

#include <Components/Managers/ResourceManager.h>
#include <Components/Selectables/ResourceCollector.h>

#include <Components/Managers/UnitTypeManager.h>

#include <Components/Selectables/Health.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


namespace
{
	static void RegisterEngineer1UnitComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Engineer1UnitComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterEngineer1UnitComponent);
}


void Engineer1UnitComponent::Initialize()
{
	//AnimationComponent Initializations
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), Quat::CreateRotationXYZ(Ang3(DEG2RAD(90), 0, DEG2RAD(180))), Vec3(0)));
	m_pAnimationComponent->SetCharacterFile("Objects/Characters/units/engineer1/engineer1.cdf");
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/engineer1.adb");
	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Walk");
	m_pAnimationComponent->SetAnimationDrivenMotion(false);
	m_pAnimationComponent->LoadFromDisk();
	m_pAnimationComponent->ResetCharacter();

	m_pAnimationComponent->EnableGroundAlignment(true);

	//StateManagerComponent Initialization
	m_pStateManagerComponent = m_pEntity->GetOrCreateComponent<UnitStateManagerComponent>();
	m_pStateManagerComponent->SetWalkSpeed(2.5f);

	//AIController Initializations
	m_pAIController = m_pEntity->GetOrCreateComponent<AIControllerComponent>();

	//SelectableComponent Initializations
	m_pSelectableComponent = m_pEntity->GetOrCreateComponent<SelectableComponent>();
	//UIItems
	m_pSelectableComponent->AddUIItem(new UIHQ1BuildItem(m_pEntity));
	m_pSelectableComponent->AddUIItem(new UIBarracks1BuildItem(m_pEntity));
	m_pSelectableComponent->AddUIItem(new UIWarehouse1BuildItem(m_pEntity));
	m_pSelectableComponent->AddUIItem(new UIHouse1BuildItem(m_pEntity));
	m_pSelectableComponent->AddUIItem(new UIFarm1BuildItem(m_pEntity));
	m_pSelectableComponent->AddUIItem(new UIWindmillBuildItem(m_pEntity));
	m_pSelectableComponent->AddUIItem(new UIBakery1BuildItem(m_pEntity));
	m_pSelectableComponent->AddUIItem(new UIAlchemy1BuildItem(m_pEntity));
	m_pSelectableComponent->AddUIItem(new UIGunPowderFactory1BuildItem(m_pEntity));
	m_pSelectableComponent->AddUIItem(new UIBulletFactory1BuildItem(m_pEntity));
	m_pSelectableComponent->AddUIItem(new UIAK47Factory1BuildItem(m_pEntity));
	m_pSelectableComponent->AddUIItem(new UILight1BuildItem(m_pEntity));

	//ActionManager Initializations
	m_pActionManagerComponent = m_pEntity->GetOrCreateComponent<ActionManagerComponent>();

	//OwnerComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetComponent<OwnerInfoComponent>();

	//AttackerComponent Initialization
	m_pUnitAnimationComponent = m_pEntity->GetOrCreateComponent<UnitAnimationComponent>();

	//////////AttackerComponent Initializations
	m_pAttackerComponent = m_pEntity->GetOrCreateComponent<AttackerComponent>();
	m_pAttackerComponent->SetDamageAmount(10.f);
	//attack info
	SUnitAttackInfo pAttckInfo;
	pAttckInfo.m_pAttackType = EAttackType::MELEE;
	pAttckInfo.bIsFollower = false;
	pAttckInfo.bIsHumanoid = true;
	pAttckInfo.m_timeBetweenAttacks = 0.7f;
	pAttckInfo.m_maxAttackDistance = 1.5f;
	m_pAttackerComponent->SetAttackInfo(pAttckInfo);

	//EngineerComponent Initializations
	m_pEngineerComponent = m_pEntity->GetOrCreateComponent<EngineerComponent>();
	//engineer info
	SEngineerInfo engineerInfo;
	engineerInfo.m_maxBuildDistance = 0.4f;
	engineerInfo.m_timeBetweenBuilds = 1.f;
	m_pEngineerComponent->SetEngineerInfo(engineerInfo);

	//CostComponent Initializations
	m_pCostComponent = m_pEntity->GetOrCreateComponent<CostComponent>();
	m_pCostComponent->SetCost(Engineer1UnitComponent::GetCost());

	//ResourceCollectorComponent Initialization
	m_pResourceCollectorComponent = m_pEntity->GetOrCreateComponent<ResourceCollectorComponent>();

	//WorkerComponent Initialization
	m_pWorkerComponent = m_pEntity->GetOrCreateComponent<WorkerComponent>();

	//UnitTypeManagerComponent
	m_pUnitTypeManagerComponent = m_pEntity->GetOrCreateComponent<UnitTypeManagerComponent>();
	m_pUnitTypeManagerComponent->SetUnitType(EUnitType::ENGINEER1);

	//HealthComponent Initialization
	m_pHealthComponent = m_pEntity->GetOrCreateComponent<HealthComponent>();
	m_pHealthComponent->SetConsumesFood(true);
}


Cry::Entity::EventFlags Engineer1UnitComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void Engineer1UnitComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];

	}break;
	case Cry::Entity::EEvent::Reset: {
		m_pAnimationComponent->ResetCharacter();

	}break;
	default:
		break;
	}
}

SResourceInfo Engineer1UnitComponent::GetCost()
{
	SResourceInfo cost;
	cost.m_moneyAmount = 10;
	cost.m_populationAmount = 1;
	cost.m_bulletAmount = 0;
	cost.m_ak47Amount = 0;
	return cost;
}

