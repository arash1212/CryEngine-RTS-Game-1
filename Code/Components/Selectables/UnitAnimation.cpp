#include "StdAfx.h"
#include "UnitAnimation.h"
#include "GamePlugin.h"

#include <Components/Selectables/Attacker.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Controller/AIController.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


namespace
{
	static void RegisterUnitAnimationComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(UnitAnimationComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterUnitAnimationComponent);
}


void UnitAnimationComponent::Initialize()
{
	//AnimationComponent Initializations
	m_pAnimationComponent = m_pEntity->GetComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();

	//Animations
	m_idleFragmentId = m_pAnimationComponent->GetFragmentId("Idle");
	m_runFragmentId = m_pAnimationComponent->GetFragmentId("Run");
	m_walkFragmentId = m_pAnimationComponent->GetFragmentId("Walk");
	m_crouchFragmentId = m_pAnimationComponent->GetFragmentId("Crouch");
	m_proneFragmentId = m_pAnimationComponent->GetFragmentId("Prone");

	m_attack1FragmentId = m_pAnimationComponent->GetFragmentId("Attack1");
	m_attack2FragmentId = m_pAnimationComponent->GetFragmentId("Attack2");
	m_attack3FragmentId = m_pAnimationComponent->GetFragmentId("Attack3");

	//StateManagerComponent Initialization
	m_pStateManagerComponent = m_pEntity->GetComponent<UnitStateManagerComponent>();

	//AIController Initializations
	m_pAIController = m_pEntity->GetComponent<AIControllerComponent>();

	//AttackerComponent Initialization
	m_pAttackerComponent = m_pEntity->GetOrCreateComponent<AttackerComponent>();
	//Attack animations
	m_pAttackAnimations.append(m_attack1FragmentId);
	m_pAttackAnimations.append(m_attack2FragmentId);
	//m_pAttackAnimations.append(m_attack3FragmentId);
}


Cry::Entity::EventFlags UnitAnimationComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void UnitAnimationComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];

		UpdateAnimations();

	}break;
	case Cry::Entity::EEvent::Reset: {;

	}break;
	default:
		break;
	}
}

void UnitAnimationComponent::UpdateAnimations()
{
	if (!m_pStateManagerComponent || !m_pAttackerComponent) {
		return;
	}

	if (bIsHumaniod) {
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

		//Update Animation
		//Idle
		if (m_pStateManagerComponent->GetStance() == EUnitStance::WALKING) {
			currentFragmentId = m_walkFragmentId;
		}

		//Walk
		else if (m_pStateManagerComponent->GetStance() == EUnitStance::CROUCH) {
			currentFragmentId = m_crouchFragmentId;
		}

		//Prone
		else if (m_pStateManagerComponent->GetStance() == EUnitStance::PRONE) {
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
}

void UnitAnimationComponent::PlayRandomAttackAnimation()
{
	m_activeFragmentId = m_attack1FragmentId;
	FragmentID attackingFragmentId = m_pAttackAnimations[MathUtils::GetRandomInt(0, m_pAttackAnimations.size() - 1)];
	m_pAnimationComponent->GetActionController()->Flush();
	IActionPtr action = new TAction<SAnimationContext>(30U, attackingFragmentId);
	m_pAnimationComponent->QueueCustomFragment(*action);
}
