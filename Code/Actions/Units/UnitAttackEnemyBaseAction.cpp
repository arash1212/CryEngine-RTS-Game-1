#include "StdAfx.h"
#include "UnitAttackEnemyBaseAction.h"
#include "GamePlugin.h"

#include <Components/Managers/ResourceManager.h>
#include <Utils/MathUtils.h>
#include <Utils/EntityUtils.h>
#include <Components/Effects/BulletTracer.h>
#include <Components/Info/OwnerInfo.h>

#include <Components/Selectables/Attacker.h>
#include <Components/Controller/AIController.h>
#include <Components/Managers/UnitStateManager.h>

UnitAttackEnemyBaseAction::UnitAttackEnemyBaseAction(IEntity* entity, IEntity* target)
{
	this->m_pEntity = entity;
	this->m_pTarget = target;
	this->m_pAttackerComponent = m_pEntity->GetComponent<AttackerComponent>();
	this->m_pAiControllerComponent = m_pEntity->GetComponent<AIControllerComponent>();
	this->m_pTargetResourceManagerComponent = target->GetComponent<ResourceManagerComponent>();

	//Set Unit stance to walking if it's running
	//if (m_pStateManagerComponent && m_pStateManagerComponent->GetStance() == EUnitStance::RUNNING) {
	//	m_pStateManagerComponent->SetStance(EUnitStance::WALKING);
	//}
}

void UnitAttackEnemyBaseAction::Execute()
{
	if (!m_pAttackerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitAttackEnemyBaseAction : (Execute) AttackerComponent is null");
		bIsDone = true;
		return;
	}
	if (!m_pAiControllerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitAttackEnemyBaseAction : (Execute) AiControllerComponent is null");
		bIsDone = true;
		return;
	}
	if (!m_pTargetResourceManagerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitAttackEnemyBaseAction : (Execute) TargetResourceManagerComponent is null");
		bIsDone = true;
		return;
	}

	if (m_pAttackerComponent->IsAttacking()) {
		return;
	}

	DynArray<IEntity*> targetResources = m_pTargetResourceManagerComponent->GetOwnedEntities();
	if (targetResources.size() > 0) {
		m_pAiControllerComponent->MoveTo(EntityUtils::GetClosetPointOnMeshBorder(m_pEntity->GetWorldPos(), targetResources[0]), true);
		m_pAiControllerComponent->LookAtWalkDirection();
	}
	else {
		m_pAiControllerComponent->StopMoving();
	}
}

void UnitAttackEnemyBaseAction::Cancel()
{
	if (!m_pAttackerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitAttackEnemyBaseAction : (Cancel) AttackerComponent is null");
		bIsDone = true;
		return;
	}
	if (!m_pAiControllerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitAttackEnemyBaseAction : (Cancel) AiControllerComponent is null");
		bIsDone = true;
		return;
	}

	m_pAttackerComponent->SetTargetEntity(nullptr);
	m_pAiControllerComponent->StopMoving();
	bIsDone = true;
}

bool UnitAttackEnemyBaseAction::IsDone()
{
	return bIsDone;
}

bool UnitAttackEnemyBaseAction::CanBeSkipped()
{
	return true;
}