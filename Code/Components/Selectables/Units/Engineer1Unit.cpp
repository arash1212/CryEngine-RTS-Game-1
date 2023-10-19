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
	m_pStateManagerComponent->SetWalkSpeed(5.f);

	//AIController Initializations
	m_pAIController = m_pEntity->GetOrCreateComponent<AIControllerComponent>();

	//SelectableComponent Initializations
	m_pSelectableComponent = m_pEntity->GetOrCreateComponent<SelectableComponent>();
	//UIItems
	m_pSelectableComponent->AddUIItem(new UIHQ1BuildItem(m_pEntity));
	m_pSelectableComponent->AddUIItem(new UIBarracks1BuildItem(m_pEntity));
	m_pSelectableComponent->AddUIItem(new UIWarehouse1BuildItem(m_pEntity));
	m_pSelectableComponent->AddUIItem(new UIHouse1BuildItem(m_pEntity));

	//ActionManager Initializations
	m_pActionManagerComponent = m_pEntity->GetOrCreateComponent<ActionManagerComponent>();

	//OwnerComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetComponent<OwnerInfoComponent>();

	//AttackerComponent Initialization
	m_pUnitAnimationComponent = m_pEntity->GetOrCreateComponent<UnitAnimationComponent>();

	//////////AttackerComponent Initializations
	m_pAttackerComponent = m_pEntity->GetOrCreateComponent<AttackerComponent>();
	//attack info
	SUnitAttackInfo pAttckInfo;
	pAttckInfo.m_pAttackType = EAttackType::MELEE;
	pAttckInfo.bIsFollower = true;
	pAttckInfo.bIsHumanoid = true;
	pAttckInfo.m_timeBetweenAttacks = 0.7f;
	pAttckInfo.m_maxAttackDistance = 2.3f;
	m_pAttackerComponent->SetAttackInfo(pAttckInfo);

	//EngineerComponent Initializations
	m_pEngineerComponent = m_pEntity->GetOrCreateComponent<EngineerComponent>();
	//engineer info
	SEngineerInfo engineerInfo;
	engineerInfo.m_maxBuildDistance = 8.f;
	engineerInfo.m_timeBetweenBuilds = 1.f;
	m_pEngineerComponent->SetEngineerInfo(engineerInfo);

	//CostComponent Initializations
	m_pCostComponent = m_pEntity->GetOrCreateComponent<CostComponent>();
	m_pCostComponent->SetCost(Engineer1UnitComponent::GetCost());

	//ResourceCollectorComponent Initialization
	m_pResourceCollectorComponent = m_pEntity->GetOrCreateComponent<ResourceCollectorComponent>();

	//**************************************Resource Attchments
	//OilBarrelAttachment Initialization
	m_pOilBarrelAttachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("oilBarrel");
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
		UpdateResourceAttachment();

	}break;
	case Cry::Entity::EEvent::Reset: {
		m_pAnimationComponent->ResetCharacter();

	}break;
	default:
		break;
	}
}

void Engineer1UnitComponent::UpdateResourceAttachment()
{
	if (!m_pResourceCollectorComponent) {
		return;
	}
	EResourceType pResourceType = m_pResourceCollectorComponent->GetCurrentResourceType();
	if (m_pResourceCollectorComponent->GetAmountOfResourceCollected() <= 0) {
		m_pOilBarrelAttachment->HideAttachment(true);
		return;
	}
	
	switch (pResourceType)
	{
	case EResourceType::Money: {
		m_pOilBarrelAttachment->HideAttachment(true);
	}break;
	case EResourceType::OIL: {
		m_pOilBarrelAttachment->HideAttachment(false);
	}break;
	default:
		break;
	}
}

SResourceInfo Engineer1UnitComponent::GetCost()
{
	SResourceInfo cost;
	cost.m_moneyAmount = 60;
	cost.m_oilAmount = 20;
	return cost;
}

