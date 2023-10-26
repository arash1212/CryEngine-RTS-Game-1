#include "StdAfx.h"
#include "Attacker.h"
#include "GamePlugin.h"

#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Controller/AIController.h>
#include <Components/Weapons/BaseWeapon.h>
#include <Components/Selectables/UnitAnimation.h>
#include <Components/Managers/ActionManager.h>
#include <Components/Effects/BulletTracer.h>

#include <Utils/MathUtils.h>
#include <Utils/EntityUtils.h>

#include <Components/Selectables/Health.h>

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

	//AIController Initialization
	m_pAIController = m_pEntity->GetComponent<AIControllerComponent>();

	//tateManagerComponent Initialization
	m_pStateManagerComponent = m_pEntity->GetComponent<UnitStateManagerComponent>();

	//WeaponComponent Initialization
	m_pWeaponComponent = m_pEntity->GetComponent<BaseWeaponComponent>();

	//UnitAnimationComponent Initialization
	m_pUnitAnimationComponent = m_pEntity->GetOrCreateComponent<UnitAnimationComponent>();

	//ActionManager Initializations
	m_pActionManagerComponent = m_pEntity->GetComponent<ActionManagerComponent>();
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
		if (m_attackTimePassed < m_pAttackInfo.m_timeBetweenAttacks) {
			m_attackTimePassed += 0.5f * DeltaTime;
		}
		if (m_attackCountResetTimePassed < m_timeBetweenAttackCountReset) {
			m_attackCountResetTimePassed += 0.5f * DeltaTime;
		}
		else {
			m_attackCount = 0;
		}

		//TODO : ATTACK ANIMATION EVENT (FOR NOT RANGED)

	}break;
	case Cry::Entity::EEvent::Reset: {
		m_pAttackTargetEntity = nullptr;
		m_pRandomAttackTarget = nullptr;

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
	if (m_attackTimePassed < m_pAttackInfo.m_timeBetweenAttacks || m_attackCount >= m_maxAttackCount) {
		return;
	}

	//Depending on this unit's attack type preform an attack 
	switch (m_pAttackInfo.m_pAttackType)
	{

	//If attacker is ranged unit
	case EAttackType::RANGED: {
		PerformRangedAttack(target);
	}break;


	//If attacker is melee unit
	case EAttackType::MELEE: {
		PerformMeleeAttack(target);
	}break;

	default:
		break;
	}

	m_attackTimePassed = 0;
	m_attackCount++;
	m_attackCountResetTimePassed = 0.f;
}


void AttackerComponent::AttackRandomTarget()
{
	if (!m_pRandomAttackTarget || m_pAttackTargetEntity || m_pStateManagerComponent->IsRunning() || m_pActionManagerComponent->IsProcessingAnAction()) {
		return;
	}

	//Attack RandomAttackTarget if it's in unit's Attack range
	if (CanAttack()) {
		Attack(m_pRandomAttackTarget);
		this->LookAt(m_pRandomAttackTarget->GetWorldPos());
		this->m_pAIController->StopMoving();
	}

	else {
		//If is not a follwer empty randomAttackTarget
		if (!m_pAttackInfo.bIsFollower) {
			m_pRandomAttackTarget = nullptr;
		}

		//If is a follwer follow random target if it's not in unit attack range
		else {
			this->m_pAIController->MoveTo(EntityUtils::GetClosetPointOnMeshBorder(m_pEntity->GetWorldPos(), m_pRandomAttackTarget), true);
			this->m_pAIController->LookAtWalkDirection();
		}
	}
}

void AttackerComponent::PerformMeleeAttack(IEntity* target)
{
	m_pUnitAnimationComponent->PlayRandomAttackAnimation();

	ApplyDamageToTarget(target);
}

void AttackerComponent::PerformRangedAttack(IEntity* target)
{
	m_pWeaponComponent->Fire(target->GetWorldPos());

	ApplyDamageToTarget(target);
}

void AttackerComponent::ApplyDamageToTarget(IEntity* target)
{
	HealthComponent* healthComponent = target->GetComponent<HealthComponent>();
	if (!healthComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "AttackerComponent : (ApplyDamageToTarget) target have to healthComonent assigned.");
		return;
	}
	healthComponent->ApplyDamage(m_damageAmount);
}

void AttackerComponent::FindRandomTarget()
{

	if (m_pRandomAttackTarget || m_pAttackTargetEntity || m_pActionManagerComponent->IsProcessingAnAction()) {
		m_pRandomAttackTarget = nullptr;
		return;
	}

	IEntityItPtr entityItPtr = gEnv->pEntitySystem->GetEntityIterator();
	entityItPtr->MoveFirst();

	while (!entityItPtr->IsEnd())
	{
		IEntity* entity = entityItPtr->Next();

		f32 distanceToTarget = EntityUtils::GetDistance(m_pEntity->GetWorldPos(), entity->GetWorldPos(), entity);
		OwnerInfoComponent* otherEntityOwnerInfo = entity->GetComponent<OwnerInfoComponent>();

		BulletTracerComponent* bulletTracerComponent = entity->GetComponent<BulletTracerComponent>();
		//Ignore entity if it's not in detection range
		if (!otherEntityOwnerInfo || distanceToTarget > m_pAttackInfo.m_detectionDistance || !otherEntityOwnerInfo->CanBeTarget() || bulletTracerComponent) {
			continue;
		}

		//set entity as randomAttackTarget if it's team is not same as this unit's team
		OwnerInfoComponent* pOwnerInfoComponent = m_pEntity->GetComponent<OwnerInfoComponent>();
		if (pOwnerInfoComponent && otherEntityOwnerInfo && otherEntityOwnerInfo->GetInfo().m_pTeam != pOwnerInfoComponent->GetInfo().m_pTeam) {
			m_pRandomAttackTarget = entity;
		}
	}
}

void AttackerComponent::LookAt(Vec3 position)
{
	if (m_pAttackInfo.bIsHumanoid) {
		ISkeletonPose* pPose = m_pAnimationComponent->GetCharacter()->GetISkeletonPose();
		IDefaultSkeleton& pDefaultSkeleton = m_pAnimationComponent->GetCharacter()->GetIDefaultSkeleton();
		int headId = pDefaultSkeleton.GetJointIDByName("miamorig:Head");
		Vec3 headPos = m_pEntity->GetWorldPos() + pPose->GetAbsJointByID(headId).t;

		Vec3 targetPos = position;
		Vec3 currentPos = m_pEntity->GetWorldPos();
		Vec3 dir = targetPos - currentPos;
		Vec3 forwardCross = m_pEntity->GetForwardDir().cross(dir.normalized());
		Vec3 rightCross = m_pEntity->GetRightDir().cross(dir.normalized());

		//Look Up/Down
		f32 dot = m_pEntity->GetForwardDir().normalized().dot(dir.normalized());
		f32 diff = 0.2f;
		int32 uInv = 1;
		if (targetPos.z < currentPos.z) {
			uInv = -1;
		}
		f32 ac = crymath::acos(dot) * uInv;
		ac -= diff;
		m_pAnimationComponent->SetMotionParameter(EMotionParamID::eMotionParamID_TurnAngle, ac);


		//Look Left/Right
		//m_pAnimationComponent->SetMotionParameter(EMotionParamID::eMotionParamID_BlendWeight, forwardCross.z);
		m_pAIController->LookAt(position);
	}
}

bool AttackerComponent::IsAttacking()
{
	if (!m_pAttackTargetEntity && !m_pRandomAttackTarget) {
		return false;
	}
	IEntity* target = m_pAttackTargetEntity ? m_pAttackTargetEntity : m_pRandomAttackTarget;
	if (!target) {
		return false;
	}

	f32 distanceToTarget = EntityUtils::GetDistance(m_pEntity->GetWorldPos(), target->GetWorldPos(), target);
	if (distanceToTarget > m_pAttackInfo.m_maxAttackDistance) {
		return false;
	}

	return true;
}

bool AttackerComponent::CanAttack()
{
	if (!m_pAttackTargetEntity && !m_pRandomAttackTarget) {
		return false;
	}
	IEntity* target = m_pAttackTargetEntity ? m_pAttackTargetEntity : m_pRandomAttackTarget;
	if (!target) {
		return false;
	}

	f32 distanceToTarget = EntityUtils::GetDistance(m_pEntity->GetWorldPos(), target->GetWorldPos(), target);
	if (distanceToTarget > m_pAttackInfo.m_maxAttackDistance) {
		return false;
	}

	return true;
}

void AttackerComponent::SetTargetEntity(IEntity* target)
{
	this->m_pRandomAttackTarget = nullptr;
	this->m_pAttackTargetEntity = target;
}

void AttackerComponent::SetDamageAmount(f32 damage)
{
	this->m_damageAmount = damage;
}

f32 AttackerComponent::GetDamageAmount()
{
	return m_damageAmount;
}

SUnitAttackInfo AttackerComponent::GetAttackInfo()
{
	return m_pAttackInfo;
}

void AttackerComponent::SetAttackInfo(SUnitAttackInfo attackInfo)
{
	m_pAttackInfo = attackInfo;
}
