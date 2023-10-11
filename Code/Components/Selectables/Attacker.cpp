#include "StdAfx.h"
#include "Attacker.h"
#include "GamePlugin.h"

#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Controller/AIController.h>
#include <Components/Weapons/BaseWeapon.h>

#include <Utils/MathUtils.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


namespace
{
	static void RegisterAttackerComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(AttackerComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterAttackerComponent);
}


void AttackerComponent::Initialize()
{
	//AnimationComponent Initialization
	m_pAnimationComponent = m_pEntity->GetComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();

	//tateManagerComponent Initialization
	m_pStateManagerComponent = m_pEntity->GetComponent<UnitStateManagerComponent>();
	
	//AIController Initialization
	m_pAIController = m_pEntity->GetComponent<AIControllerComponent>();

	//WeaponComponent Initialization
	m_pWeaponComponent = m_pEntity->GetComponent<BaseWeaponComponent>();
}


Cry::Entity::EventFlags AttackerComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void AttackerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		f32 DeltaTime = event.fParam[0];
 
		//Target/Attack
		FindRandomTarget();
		AttackRandomTarget();

		//Timers
		if (m_attackTimePassed < m_timeBetweenAttacks) {
			m_attackTimePassed += 0.5f * DeltaTime;
		}
		if (m_attackCountResetTimePassed < m_timeBetweenAttackCountReset) {
			m_attackCountResetTimePassed += 0.5f * DeltaTime;
		}
		else {
			m_attackCount = 0;
		}

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}


void AttackerComponent::Attack(IEntity* target)
{
	if (!target) {
		return;
	}
	if (m_attackTimePassed < m_timeBetweenAttacks || m_attackCount >= m_maxAttackCount) {
		return;
	}

	//Fire Weapon
	if (m_pWeaponComponent) {
		m_pWeaponComponent->Fire(target->GetWorldPos());
	}

	/*
	FragmentID id = m_pAnimationComponent->GetFragmentId("Run");
	CryCharAnimationParams animationParams;
	animationParams.m_nLayerID = 1;
	animationParams.m_fPlaybackWeight = 0.7f;
	m_pAnimationComponent->GetCharacter()->GetISkeletonAnim()->StartAnimationById(id, animationParams);
	*/

	//Play Attack Animation
	if (m_pAttackAnimations.size() > 0) {
		FragmentID attackingFragmentId = m_pAttackAnimations[MathUtils::GetRandomInt(0, m_pAttackAnimations.size())];
		m_pAnimationComponent->GetActionController()->Flush();
		IActionPtr action = new TAction<SAnimationContext>(30U, attackingFragmentId);
		m_pAnimationComponent->QueueCustomFragment(*action);
		bUpdatedAnimation = true;
	}

	m_attackTimePassed = 0;
	m_attackCount++;
	m_attackCountResetTimePassed = 0.f;
}


void AttackerComponent::AttackRandomTarget()
{
	if (!m_pRandomAttackTarget || m_pAttackTargetEntity || m_pStateManagerComponent->IsRunning()) {
		return;
	}

	f32 distanceToTarget = m_pEntity->GetWorldPos().GetDistance(m_pRandomAttackTarget->GetWorldPos());
	if (distanceToTarget < m_pAttackInfo.m_maxAttackDistance) {
		Attack(m_pRandomAttackTarget);
	}
	else {
		m_pRandomAttackTarget = nullptr;
	}
}

void AttackerComponent::FindRandomTarget()
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

		if (entityOwnerInfo->GetInfo().m_pTeam != m_pEntity->GetComponent<OwnerInfoComponent>()->GetInfo().m_pTeam) {
			m_pRandomAttackTarget = entity;
		}
	}
}


void AttackerComponent::LookAt(Vec3 position)
{
	if (bIsHumanoid) {
		ISkeletonPose* pPose = m_pAnimationComponent->GetCharacter()->GetISkeletonPose();
		IDefaultSkeleton& pDefaultSkeleton = m_pAnimationComponent->GetCharacter()->GetIDefaultSkeleton();
		int headId = pDefaultSkeleton.GetJointIDByName("miamorig:Head");
		Vec3 headPos = m_pEntity->GetWorldPos() + pPose->GetAbsJointByID(headId).t;

		Vec3 targetPos = position;
		Vec3 currentPos = headPos;
		Vec3 dir = targetPos - currentPos;
		Vec3 forwardCross = m_pEntity->GetForwardDir().cross(dir.normalized());
		Vec3 rightCross = m_pEntity->GetRightDir().cross(dir.normalized());

		//Look Up/Down
		int32 uInv = 1;
		f32 diff = 0.2f;

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
		m_pAIController->LookAt(position);
	}
}

bool AttackerComponent::IsUpdatedAnimation()
{
	return bUpdatedAnimation;
}

void AttackerComponent::SetUpdatedAnimation(bool updatedAnimation)
{
	this->bUpdatedAnimation = updatedAnimation;
}

void AttackerComponent::SetTargetEntity(IEntity* target)
{
	this->m_pRandomAttackTarget = nullptr;
	this->m_pAttackTargetEntity = target;
}

IEntity* AttackerComponent::GetAttackTarget()
{
	return m_pAttackTargetEntity;
}

void AttackerComponent::SetAttackTarget(IEntity* attacktTarget)
{
	this->m_pAttackTargetEntity = attacktTarget;
}

IEntity* AttackerComponent::GetRandomAttackTarget()
{
	return m_pRandomAttackTarget;
}

void AttackerComponent::SetRandomAttackTarget(IEntity* randomTarget)
{
	this->m_pRandomAttackTarget = randomTarget;
}

void AttackerComponent::SetIsHumanoid(bool isHumanoid)
{
	this->bIsHumanoid = isHumanoid;
}

bool AttackerComponent::IsHumanoid()
{
	return bIsHumanoid;
}

void AttackerComponent::SetTimeBetweenAttack(f32 timeBetweenAttacks)
{
	this->m_timeBetweenAttacks = timeBetweenAttacks;
}

bool AttackerComponent::IsAttacking()
{
	if (!m_pAttackTargetEntity && !m_pRandomAttackTarget) {
		return false;
	}
	IEntity* target = m_pAttackTargetEntity ? m_pAttackTargetEntity : m_pRandomAttackTarget;

	f32 distanceToTarget = m_pEntity->GetWorldPos().GetDistance(target->GetWorldPos());
	if (distanceToTarget > m_pAttackInfo.m_maxAttackDistance) {
		return false;
	}

	return true;
}

void AttackerComponent::SetAttackAnimations(DynArray<FragmentID> attackAnimations)
{
	this->m_pAttackAnimations = attackAnimations;
}

SUnitAttackInfo AttackerComponent::GetAttackInfo()
{
	return m_pAttackInfo;
}

void AttackerComponent::SetAttackInfo(SUnitAttackInfo attackInfo)
{
	m_pAttackInfo = attackInfo;
}
