#include "StdAfx.h"
#include "AttackAction.h"
#include "GamePlugin.h"

#include <Components/Selectables/Attacker.h>
#include <Components/Controller/AIController.h>

AttackAction::AttackAction(IEntity* entity, IEntity* target)
{
	this->m_pEntity = entity;
	this->m_pTarget = target;
}

void AttackAction::Execute()
{
	AttackerComponent* attacker = m_pEntity->GetComponent<AttackerComponent>();
	AIControllerComponent* pAiController = m_pEntity->GetComponent<AIControllerComponent>();
	if (attacker) {
		f32 distanceToTarget = m_pEntity->GetWorldPos().GetDistance(m_pTarget->GetWorldPos());
		if (distanceToTarget > attacker->GetAttackInfo().m_maxAttackDistance) {
			attacker->SetTargetEntity(m_pTarget);
			pAiController->MoveTo(m_pTarget->GetWorldPos(), false);
			pAiController->LookAtWalkDirection();
		}
		else {
			attacker->Attack(m_pTarget);
			attacker->LookAt(m_pTarget->GetWorldPos());
		}
	}
}

void AttackAction::Cancel()
{
	AttackerComponent* attacker = m_pEntity->GetComponent<AttackerComponent>();
	if (attacker) {
		attacker->SetTargetEntity(nullptr);
		//unit->MoveTo(m_pEntity->GetWorldPos(), false);
		bIsDone = true;
	}
}

bool AttackAction::IsDone()
{
	return bIsDone;
}