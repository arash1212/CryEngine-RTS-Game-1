#include "StdAfx.h"
#include "Zombie1Unit.h"
#include "GamePlugin.h"

#include <Components/Selectables/Attacker.h>

#include <Components/Selectables/Selectable.h>
#include <UIItems/IBaseUIItem.h>
#include <UIItems/Items/UICancelItem.h>
#include <UIItems/Items/UIChangeStanceItem.h>
#include <UIItems/Items/Buildings/UIHQ1BuildItem.h>
#include <Actions/Units/UnitWanderingRandomlyAction.h>

#include <Components/Controller/AIController.h>
#include <Components/Managers/ActionManager.h>
#include <Components/Managers/UnitStateManager.h>

#include <CryAnimation/ICryAnimation.h>
#include <Components/Weapons/BaseWeapon.h>

#include <Components/Info/OwnerInfo.h>

#include <Components/Selectables/UnitAnimation.h>
#include <Components/Selectables/Cost.h>

#include <Components/Managers/ResourceManager.h>

#include <Components/Managers/UnitTypeManager.h>

#include <Components/Selectables/Health.h>
#include <Components/Selectables/Visibility.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


namespace
{
	static void RegisterZombie1UnitComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Zombie1UnitComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterZombie1UnitComponent);
}


void Zombie1UnitComponent::Initialize()
{
	//AnimationComponent Initializations
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), Quat::CreateRotationXYZ(Ang3(DEG2RAD(90), 0, DEG2RAD(180))), Vec3(0)));
	m_pAnimationComponent->SetCharacterFile("objects/characters/units/zombie1/zombie1.cdf");
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/zombie1.adb");
	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Walk");
	m_pAnimationComponent->SetAnimationDrivenMotion(false);
	m_pAnimationComponent->LoadFromDisk();
	m_pAnimationComponent->ResetCharacter();

	m_pAnimationComponent->EnableGroundAlignment(true);

	//StateManagerComponent Initialization
	m_pStateManagerComponent = m_pEntity->GetOrCreateComponent<UnitStateManagerComponent>();
	m_pStateManagerComponent->SetWalkSpeed(1.5f);
	m_pStateManagerComponent->SetRunSpeed(3.0f);

	//AIController Initializations
	m_pAIController = m_pEntity->GetOrCreateComponent<AIControllerComponent>();

	//SelectableComponent Initializations
	m_pSelectableComponent = m_pEntity->GetOrCreateComponent<SelectableComponent>();
	//UIItems

	//ActionManager Initializations
	m_pActionManagerComponent = m_pEntity->GetOrCreateComponent<ActionManagerComponent>();

	//OwnerComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetComponent<OwnerInfoComponent>();

	//AttackerComponent Initialization
	m_pUnitAnimationComponent = m_pEntity->GetOrCreateComponent<UnitAnimationComponent>();

	//////////AttackerComponent Initializations
	m_pAttackerComponent = m_pEntity->GetOrCreateComponent<AttackerComponent>();
	m_pAttackerComponent->SetDamageAmount(7.f);
	//attack info
	SUnitAttackInfo pAttckInfo;
	pAttckInfo.m_pAttackType = EAttackType::MELEE;
	pAttckInfo.bIsFollower = true;
	pAttckInfo.bIsHumanoid = true;
	pAttckInfo.m_timeBetweenAttacks = 0.7f;
	pAttckInfo.m_maxAttackDistance = 0.7f;
	pAttckInfo.m_detectionDistance = 35.f;
	m_pAttackerComponent->SetAttackInfo(pAttckInfo);

	/////////CostComponent Initializations
	m_pCostComponent = m_pEntity->GetOrCreateComponent<CostComponent>();
	m_pCostComponent->SetCost(Zombie1UnitComponent::GetDescription().price);

	//UnitTypeManagerComponent
	m_pUnitTypeManagerComponent = m_pEntity->GetOrCreateComponent<UnitTypeManagerComponent>();
	m_pUnitTypeManagerComponent->SetUnitType(EUnitType::ZOMBIE1);

	//HealthComponent Initialization
	m_pHealthComponent = m_pEntity->GetOrCreateComponent<HealthComponent>();
	m_pHealthComponent->SetMaxHealth(100);

	//VisibilityComponent Initialization
	m_pVisibilityComponent = m_pEntity->GetOrCreateComponent<VisibilityComponent>();

	m_pEntity->SetName("Unit-Zombie-1");
}


Cry::Entity::EventFlags Zombie1UnitComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void Zombie1UnitComponent::ProcessEvent(const SEntityEvent& event)
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

SDescription Zombie1UnitComponent::GetDescription()
{
	SResourceInfo price;
	price.m_moneyAmount = 5;
	price.m_populationAmount = 0;

	SDescription m_pDescription;
	m_pDescription.sName = "Zombie 1";
	m_pDescription.sDescription = "Zombie 1 Unit.";
	m_pDescription.sBuyDescription = "Train Zombie 1 Unit.";
	m_pDescription.price = price;
	m_pDescription.sIcon = "zombie_1_icon.png";

	return m_pDescription;
}

IBaseInfoPanelUIItem* Zombie1UnitComponent::GetInfoPanelItem()
{
	return new UIUnitInfoPanelItem(m_pEntity, "zombie_1_icon.png");
}
