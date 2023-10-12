#include "StdAfx.h"
#include "Soldier1Unit.h"
#include "GamePlugin.h"

#include <Components/Selectables/Attacker.h>

#include <Components/Selectables/Selectable.h>
#include <Components/Controller/AIController.h>
#include <Components/Managers/ActionManager.h>
#include <Components/Managers/UnitStateManager.h>

#include <CryAnimation/ICryAnimation.h>
#include <Components/Weapons/BaseWeapon.h>

#include <Components/Info/OwnerInfo.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


namespace
{
	static void RegisterUnitComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Soldier1UnitComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterUnitComponent);
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

	//Animations
	m_idleFragmentId = m_pAnimationComponent->GetFragmentId("Idle");
	m_runFragmentId = m_pAnimationComponent->GetFragmentId("Run");
	m_walkFragmentId = m_pAnimationComponent->GetFragmentId("Walk");
	m_crouchFragmentId = m_pAnimationComponent->GetFragmentId("Crouch");
	m_proneFragmentId = m_pAnimationComponent->GetFragmentId("Prone");

	m_attack1FragmentId = m_pAnimationComponent->GetFragmentId("Attack1");

	//AnimationComponent Initializations
	m_pSelectableComponent = m_pEntity->GetOrCreateComponent<SelectableComponent>();

	//AIController Initializations
	m_pAIController = m_pEntity->GetOrCreateComponent<AIControllerComponent>();

	//ActionManager Initializations
	m_pActionManagerComponent = m_pEntity->GetOrCreateComponent<ActionManagerComponent>();

	//WeaponComponent Initialization
	m_pWeaponComponent = m_pEntity->GetOrCreateComponent<BaseWeaponComponent>();
	m_pWeaponComponent->Draw();

	//OwnerComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetOrCreateComponent<OwnerInfoComponent>();
	m_pOwnerInfoComponent->SetTeam(EPlayerTeam::TEAM6);

	//StateManagerComponent Initialization
	m_pStateManagerComponent = m_pEntity->GetOrCreateComponent<UnitStateManagerComponent>();

	//Attacker
	m_pAttackerComponent = m_pEntity->GetOrCreateComponent<AttackerComponent>();
	m_pAttackerComponent->SetIsMelee(false);
	m_pAttackerComponent->SetIsHumanoid(true);
	//attack animations
	DynArray<FragmentID> attackAnimations;
	attackAnimations.append(m_attack1FragmentId);
	m_pAttackerComponent->SetAttackAnimations(attackAnimations);
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

		UpdateAnimations();

		m_pActionManagerComponent->ProcessActions();

	}break;
	case Cry::Entity::EEvent::Reset: {
		m_pAnimationComponent->ResetCharacter();

	}break;
	default:
		break;
	}
}

/*=============================================================================================================================================
																	ANIMATIONS
==============================================================================================================================================*/

void Soldier1UnitComponent::UpdateAnimations()
{
	if (!m_pStateManagerComponent) {
		return;
	}

	/////////////////////////////////////////
	m_pAnimationComponent->SetMotionParameter(EMotionParamID::eMotionParamID_TravelSpeed, m_pAIController->IsMoving() ? 3.f : 0.f);

	//Run/Walk BlendSpaces
	Vec3 forwardVector = m_pEntity->GetForwardDir().normalized();
	Vec3 rightVector = m_pEntity->GetRightDir().normalized();
	Vec3 velocity = m_pAIController->GetVelocity().normalized();

	float forwardDot = velocity.dot(forwardVector);
	float rightDot = velocity.dot(rightVector);

	int32 inv = rightDot < 0 ? 1 : -1;
	m_pAnimationComponent->SetMotionParameter(EMotionParamID::eMotionParamID_TravelAngle, crymath::acos(forwardDot) * inv);
	/////////////////////////////////////////

	//Update Animation
	//Idle
	FragmentID currentFragmentId;
	if (m_pStateManagerComponent->GetStance() == EUnitStance::WALKING) {
		currentFragmentId = m_walkFragmentId;
	}

	//Walk
	else if ( m_pStateManagerComponent->GetStance()==EUnitStance::CROUCH) {
		currentFragmentId = m_crouchFragmentId;
	}

	//Prone
	else if(m_pStateManagerComponent->GetStance() == EUnitStance::PRONE) {
		currentFragmentId = m_proneFragmentId;
	}

	//Run
	else if (m_pStateManagerComponent->GetStance() == EUnitStance::RUNNING) {
		currentFragmentId = m_runFragmentId;
	}

	if (m_activeFragmentId != currentFragmentId) {
		m_activeFragmentId = currentFragmentId;
		m_pAnimationComponent->QueueFragmentWithId(m_activeFragmentId);
	}
}

/*=============================================================================================================================================
																	ACTIONS
==============================================================================================================================================*/