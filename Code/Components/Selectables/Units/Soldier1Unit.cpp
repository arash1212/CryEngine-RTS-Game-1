#include "StdAfx.h"
#include "Soldier1Unit.h"
#include "GamePlugin.h"

#include <Components/Selectables/Attacker.h>

#include <Components/Selectables/Selectable.h>
#include <UIItems/IBaseUIItem.h>
#include <UIItems/Items/UICancelItem.h>
#include <UIItems/Items/UIChangeStanceItem.h>
#include <UIItems/Items/Buildings/UIHQ1BuildItem.h>

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

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


namespace
{
	static void RegisterSoldier1UnitComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Soldier1UnitComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterSoldier1UnitComponent);
}


void Soldier1UnitComponent::Initialize()
{
	//AnimationComponent Initializations
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), Quat::CreateRotationXYZ(Ang3(DEG2RAD(90), 0, DEG2RAD(180))), Vec3(0)));
	m_pAnimationComponent->SetCharacterFile("Objects/Characters/units/soldier1/soldier_1.cdf");
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/soldier1.adb");
	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Walk");
	m_pAnimationComponent->SetAnimationDrivenMotion(false);
	m_pAnimationComponent->LoadFromDisk();
	m_pAnimationComponent->ResetCharacter();

	m_pAnimationComponent->EnableGroundAlignment(true);

	//StateManagerComponent Initialization
	m_pStateManagerComponent = m_pEntity->GetOrCreateComponent<UnitStateManagerComponent>();
	m_pStateManagerComponent->SetWalkSpeed(5.f);

	//AIController Initializations
	m_pAIController = m_pEntity->GetOrCreateComponent<AIControllerComponent>();

	//SelectableComponent Initializations
	m_pSelectableComponent = m_pEntity->GetOrCreateComponent<SelectableComponent>();
	//UIItems

	//ActionManager Initializations
	m_pActionManagerComponent = m_pEntity->GetOrCreateComponent<ActionManagerComponent>();

	//WeaponComponent Initialization
	m_pWeaponComponent = m_pEntity->GetOrCreateComponent<BaseWeaponComponent>();
	m_pWeaponComponent->Draw();

	//OwnerComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetComponent<OwnerInfoComponent>();

	//UnitAnimationComponent Initialization
	m_pUnitAnimationComponent = m_pEntity->GetOrCreateComponent<UnitAnimationComponent>();

	//AttackerComponent Initialization
	m_pAttackerComponent = m_pEntity->GetOrCreateComponent<AttackerComponent>();
	m_pAttackerComponent->SetDamageAmount(4.f);
	//attack info
	SUnitAttackInfo pAttckInfo;
	pAttckInfo.m_pAttackType = EAttackType::RANGED;
	pAttckInfo.bIsHumanoid = true;
	pAttckInfo.bIsFollower = false;
	pAttckInfo.m_maxAttackDistance = 25.f;
	m_pAttackerComponent->SetAttackInfo(pAttckInfo);

	//CostComponent Initializations
	m_pCostComponent = m_pEntity->GetOrCreateComponent<CostComponent>();
	m_pCostComponent->SetCost(Soldier1UnitComponent::GetCost());

	//UnitTypeManagerComponent
	m_pUnitTypeManagerComponent = m_pEntity->GetOrCreateComponent<UnitTypeManagerComponent>();
	m_pUnitTypeManagerComponent->SetUnitType(EUnitType::SOLDIER1);

	//HealthComponent Initialization
	m_pHealthComponent = m_pEntity->GetOrCreateComponent<HealthComponent>();
	m_pHealthComponent->SetConsumesFood(true);
}


Cry::Entity::EventFlags Soldier1UnitComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void Soldier1UnitComponent::ProcessEvent(const SEntityEvent& event)
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

SResourceInfo Soldier1UnitComponent::GetCost()
{
	SResourceInfo cost;
	cost.m_moneyAmount = 20;
	cost.m_populationAmount = 1;
	cost.m_bulletAmount = 5;
	cost.m_ak47Amount = 1;
	return cost;
}