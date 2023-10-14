#include "StdAfx.h"
#include "AttackAction.h"
#include "GamePlugin.h"

#include <Components/Selectables/Attacker.h>
#include <Components/Controller/AIController.h>
#include <Components/Managers/UnitStateManager.h>

AttackAction::AttackAction(IEntity* entity, IEntity* target)
{
	this->m_pEntity = entity;
	this->m_pTarget = target;
	this->m_pAttackerComponent = m_pEntity->GetComponent<AttackerComponent>();
	this->m_pAiControllerComponent = m_pEntity->GetComponent<AIControllerComponent>();
	this->m_pStateManagerComponent = m_pEntity->GetComponent<UnitStateManagerComponent>();

	//Set Unit stance to walking if it's running
	if (m_pStateManagerComponent&& m_pStateManagerComponent->GetStance()==EUnitStance::RUNNING) {
		m_pStateManagerComponent->SetStance(EUnitStance::WALKING);
	}
}

void AttackAction::Execute()
{
	if (!m_pAttackerComponent){
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "AttackAction : (Execute) AttackerComponent is null");
		bIsDone = true;
		return;
	}
	if (!m_pAiControllerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "AttackAction : (Execute) AiControllerComponent is null");
		bIsDone = true;
		return;
	}

	this->m_pAttackerComponent->SetTargetEntity(m_pTarget);

	if (m_pAttackerComponent->CanAttack()) {
		m_pAttackerComponent->Attack(m_pTarget);
		m_pAttackerComponent->LookAt(m_pTarget->GetWorldPos());
	}
	else {
		m_pAiControllerComponent->MoveTo(m_pTarget->GetWorldPos(), false);
		m_pAiControllerComponent->LookAtWalkDirection();
	}
}

void AttackAction::Cancel()
{
	if (!m_pAttackerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "AttackAction : (Cancel) AttackerComponent is null");
		bIsDone = true;
		return;
	}
	if (!m_pAiControllerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "AttackAction : (Cancel) AiControllerComponent is null");
		bIsDone = true;
		return;
	}

	m_pAttackerComponent->SetTargetEntity(nullptr);
	m_pAiControllerComponent->StopMoving();
	bIsDone = true;
}

bool AttackAction::IsDone()
{
	return bIsDone;
}