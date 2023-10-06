#include "StdAfx.h"
#include "BaseUnit.h"
#include "GamePlugin.h"

#include <Components/Selectables/Selectable.h>
#include <Components/Controller/AIController.h>
#include <Components/Action/ActionManager.h>

#include <CryAnimation/ICryAnimation.h>
#include <Components/Weapons/BaseWeapon.h>

#include <Components/Info/OwnerInfo.h>

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

	//WeaponComponent Initialization
	m_pWeaponComponent = m_pEntity->GetOrCreateComponent<BaseWeaponComponent>();
	m_pWeaponComponent->Draw();

	//OwnerComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetOrCreateComponent<OwnerInfoComponent>();
	m_pOwnerInfoComponent->SetTeam(EPlayerTeam::TEAM6);
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

		//Target/Attack
		UpdateLookAtPosition();
		FindRandomTarget();
		Attack();
		

	}break;
	case Cry::Entity::EEvent::Reset: {
		m_pSelectableComponent->DeSelect();
		m_pAnimationComponent->ResetCharacter();
		m_pAttackTargetEntity = nullptr;
		m_pRandomAttackTarget = nullptr;

	}break;
	default:
		break;
	}
}

void BaseUnitComponent::UpdateAnimations()
{
	//Update Animation
	FragmentID currentFragmentId;
	if (!m_pAIController->IsMoving()) {
		currentFragmentId = m_idleFragmentId;
	}
	else if (m_pAIController->IsMoving()) {
		m_pAnimationComponent->SetMotionParameter(EMotionParamID::eMotionParamID_TravelSpeed, 3);

		Vec3 forwardVector = m_pEntity->GetForwardDir().normalized();
		Vec3 rightVector = m_pEntity->GetRightDir().normalized();
		Vec3 velocity = m_pAIController->GetVelocity().normalized();

		float forwardDot = velocity.dot(forwardVector);
		float rightDot = velocity.dot(rightVector);

		int32 inv = rightDot < 0 ? 1 : -1;
		m_pAnimationComponent->SetMotionParameter(EMotionParamID::eMotionParamID_TravelAngle, crymath::acos(forwardDot) * inv);
		currentFragmentId = m_runFragmentId;
	}
	if (m_activeFragmentId != currentFragmentId) {
		m_activeFragmentId = currentFragmentId;
		m_pAnimationComponent->QueueFragmentWithId(m_activeFragmentId);
	}
}

void BaseUnitComponent::Attack()
{
	if (!m_pAttackTargetEntity && !m_pRandomAttackTarget) {
		return;
	}

	//Get Target
	IEntity* targetEntity = m_pAttackTargetEntity ? m_pAttackTargetEntity : m_pRandomAttackTarget;
	f32 distanceToTarget = m_pEntity->GetWorldPos().GetDistance(targetEntity->GetWorldPos());
	OwnerInfoComponent* ownerInfo = targetEntity->GetComponent<OwnerInfoComponent>();
	if (targetEntity && distanceToTarget <= m_pAttackInfo.m_maxAttackDistance) {
		m_pWeaponComponent->Fire(targetEntity->GetWorldPos());
	}
	else if (distanceToTarget > m_pAttackInfo.m_maxAttackDistance || !ownerInfo) {
		m_pRandomAttackTarget = nullptr;
	}
}

void BaseUnitComponent::UpdateLookAtPosition()
{
	if (m_pAttackTargetEntity || m_pRandomAttackTarget) {
		//Get Target
		IEntity* targetEntity = m_pAttackTargetEntity ? m_pAttackTargetEntity : m_pRandomAttackTarget;
		if (!targetEntity) {
			return;
		}

		Vec3 targetPos = targetEntity->GetWorldPos();
		Vec3 currentPos = m_pEntity->GetWorldPos();
		Vec3 dir = targetPos - currentPos;
		Vec3 forwardCross = m_pEntity->GetForwardDir().cross(dir.normalized());
		Vec3 rightCross = m_pEntity->GetRightDir().cross(dir.normalized());

		//Look Up/Down
		int32 uInv = 1;
		f32 diff = 0.0f;
		if (targetPos.x > currentPos.x + 2 || targetPos.x < currentPos.x - 2) {
			if (targetPos.x > currentPos.x) {
				uInv = -1;
			}
			else {
				diff *= -1;
			}
			m_pAnimationComponent->SetMotionParameter(EMotionParamID::eMotionParamID_TurnAngle, (rightCross.x + (diff * 2)) * uInv);
		}
		else {
			if (targetPos.y < currentPos.y) {
				uInv = -1;
			}
			m_pAnimationComponent->SetMotionParameter(EMotionParamID::eMotionParamID_TurnAngle, (forwardCross.x - diff) * uInv);
		}

		//Look Left/Right
		//m_pAnimationComponent->SetMotionParameter(EMotionParamID::eMotionParamID_BlendWeight, forwardCross.z);
		m_pAIController->LookAt(targetEntity->GetWorldPos());
	}
	else if (!m_pAttackTargetEntity && !m_pRandomAttackTarget) {
		m_pAIController->LookAtWalkDirection();
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

void BaseUnitComponent::SetTargetEntity(IEntity* target)
{
	this->m_pRandomAttackTarget = nullptr;
	this->m_pAttackTargetEntity = target;
}

void BaseUnitComponent::FindRandomTarget()
{
	if (m_pRandomAttackTarget || m_pAttackTargetEntity) {
		return;
	}

	IEntityItPtr entityItPtr = gEnv->pEntitySystem->GetEntityIterator();
	entityItPtr->MoveFirst();

	while (!entityItPtr->IsEnd())
	{
		IEntity* entity = entityItPtr->Next();
		f32 distanceToTarget = m_pEntity->GetWorldPos().GetDistance(entity->GetWorldPos());
		OwnerInfoComponent* entityOwnerInfo = entity->GetComponent<OwnerInfoComponent>();
		if (!entityOwnerInfo || distanceToTarget > m_pAttackInfo.m_maxAttackDistance) {
			continue;
		}

		if (entityOwnerInfo->GetInfo().m_pTeam != m_pOwnerInfoComponent->GetInfo().m_pTeam) {
			m_pRandomAttackTarget = entity;
		}
	}
}

SUnitAttackInfo BaseUnitComponent::GetAttackInfo()
{
	return m_pAttackInfo;
}
