#include "StdAfx.h"
#include "Zombie1Unit.h"
#include "GamePlugin.h"

#include <Components/Selectables/Attacker.h>

#include <Components/Selectables/Selectable.h>
#include <Components/Controller/AIController.h>
#include <Components/Action/ActionManager.h>
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

	//Animations
	m_idleFragmentId = m_pAnimationComponent->GetFragmentId("Idle");
	m_runFragmentId = m_pAnimationComponent->GetFragmentId("Run");
	m_walkFragmentId = m_pAnimationComponent->GetFragmentId("Walk");

	m_attack1FragmentId = m_pAnimationComponent->GetFragmentId("Attack1");
	m_attack2FragmentId = m_pAnimationComponent->GetFragmentId("Attack2");
	m_attack3FragmentId = m_pAnimationComponent->GetFragmentId("Attack3");
	//m_crouchFragmentId = m_pAnimationComponent->GetFragmentId("Crouch");
	//m_proneFragmentId = m_pAnimationComponent->GetFragmentId("Prone");

	//AnimationComponent Initializations
	m_pSelectableComponent = m_pEntity->GetOrCreateComponent<SelectableComponent>();

	//AIController Initializations
	m_pAIController = m_pEntity->GetOrCreateComponent<AIControllerComponent>();

	//ActionManager Initializations
	m_pActionManagerComponent = m_pEntity->GetOrCreateComponent<ActionManagerComponent>();

	//OwnerComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetOrCreateComponent<OwnerInfoComponent>();
	m_pOwnerInfoComponent->SetTeam(EPlayerTeam::TEAM6);

	//StateManagerComponent Initialization
	m_pStateManagerComponent = m_pEntity->GetOrCreateComponent<UnitStateManagerComponent>();

	//////////AttackerComponent Initializations
	m_pAttackerComponent = m_pEntity->GetOrCreateComponent<AttackerComponent>();
	m_pAttackerComponent->SetIsHumanoid(true);
	m_pAttackerComponent->SetTimeBetweenAttack(0.7f);
	//attack info
	SUnitAttackInfo attackInfo;
	attackInfo.m_maxAttackDistance = 2.f;
	m_pAttackerComponent->SetAttackInfo(attackInfo);
	//attack animations
	DynArray<FragmentID> attackAnimations;
	attackAnimations.append(m_attack1FragmentId);
	attackAnimations.append(m_attack2FragmentId);
	attackAnimations.append(m_attack3FragmentId);
	m_pAttackerComponent->SetAttackAnimations(attackAnimations);


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

		UpdateAnimations();

		m_pActionManagerComponent->ProcessActions();

	}break;
	case Cry::Entity::EEvent::Reset: {
		m_pSelectableComponent->DeSelect();
		m_pAnimationComponent->ResetCharacter();
		m_pStateManagerComponent->SetStance(EUnitStance::STANDING);

	}break;
	default:
		break;
	}
}

/*=============================================================================================================================================
																	ANIMATIONS
==============================================================================================================================================*/

void Zombie1UnitComponent::UpdateAnimations()
{
	if (!m_pStateManagerComponent || !m_pAttackerComponent) {
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
	FragmentID currentFragmentId;

	//To update activeFragmentId after attacking
	if(m_pAttackerComponent->IsUpdatedAnimation()){
		m_activeFragmentId = m_attack1FragmentId;
		m_pAttackerComponent->SetUpdatedAnimation(false);
	}

	if (m_pStateManagerComponent->GetStance() == EUnitStance::STANDING && m_pStateManagerComponent->GetState() != EUnitState::RUN) {
		currentFragmentId = m_walkFragmentId;
	}

	//Run
	else {
		currentFragmentId = m_runFragmentId;
	}

	if (m_activeFragmentId != currentFragmentId) {
		m_activeFragmentId = currentFragmentId;
		m_pAnimationComponent->QueueFragmentWithId(m_activeFragmentId);
	}
}
