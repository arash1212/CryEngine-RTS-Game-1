#include "StdAfx.h"
#include "BaseUnit.h"
#include "GamePlugin.h"

#include <Components/Selectables/Selectable.h>
#include <Components/Controller/AIController.h>
#include <Components/Action/ActionManager.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterBaseUnitComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(BaseUnitComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterBaseUnitComponent);
}

void BaseUnitComponent::Initialize()
{
	//AnimationComponent Initializations
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), Quat::CreateRotationXYZ(Ang3(DEG2RAD(90), 0, DEG2RAD(180))), Vec3(0)));
	m_pAnimationComponent->SetCharacterFile("Objects/Characters/units/soldier1/soldier_1.cdf");
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/soldier1.adb");
	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Idle");
	m_pAnimationComponent->SetAnimationDrivenMotion(false);
	m_pAnimationComponent->LoadFromDisk();
	m_pAnimationComponent->ResetCharacter();

	//Animations
	m_idleFragmentId = m_pAnimationComponent->GetFragmentId("Idle");
	m_runFragmentId = m_pAnimationComponent->GetFragmentId("Run");

	//AnimationComponent Initializations
	m_pSelectableComponent = m_pEntity->GetOrCreateComponent<SelectableComponent>();

	//AIController Initializations
	m_pAIController = m_pEntity->GetOrCreateComponent<AIControllerComponent>();

	//ActionManager Initializations
	m_pActionManagerComponent = m_pEntity->GetOrCreateComponent<ActionManagerComponent>();
}


Cry::Entity::EventFlags BaseUnitComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void BaseUnitComponent::ProcessEvent(const SEntityEvent& event)
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

	}break;
	default:
		break;
	}
}

void BaseUnitComponent::UpdateAnimations()
{
	FragmentID currentFragmentId;

	if (!m_pAIController->IsMoving()) {
		currentFragmentId = m_idleFragmentId;
	}
	else if (m_pAIController->IsMoving()) {

		Vec3 forwardDir = m_pEntity->GetForwardDir();
		Vec3 rightDir = m_pEntity->GetRightDir();
		Vec3 velocity = m_pAIController->GetVelocity();

		f32 forwardDot = velocity.dot(forwardDir);
		f32 rightDot = velocity.dot(rightDir);

		int32 inv = rightDot > 0 ? 1 : -1;

		m_pAnimationComponent->SetMotionParameter(EMotionParamID::eMotionParamID_TravelSpeed, 3);
		m_pAnimationComponent->SetMotionParameter(EMotionParamID::eMotionParamID_TurnAngle, crymath::acos(forwardDot) * inv);
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

void BaseUnitComponent::MoveTo(Vec3 position)
{
	m_pAIController->MoveTo(position);
}

void BaseUnitComponent::StopMoving()
{
	m_pAIController->StopMoving();
}
