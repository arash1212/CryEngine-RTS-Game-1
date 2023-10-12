#include "StdAfx.h"
#include "MoveAction.h"
#include "GamePlugin.h"

#include <Components/Selectables/Attacker.h>
#include <Components/Controller/AIController.h>
#include <Components/Managers/UnitStateManager.h>

MoveAction::MoveAction(IEntity* entity, Vec3 movePosition, bool run)
{
	this->m_pEntity = entity;
	this->m_movePosition = movePosition;
	this->bRun = run;
	this->m_pAttackerComponent = m_pEntity->GetComponent<AttackerComponent>();
	this->m_pAiControllerComponent = m_pEntity->GetComponent<AIControllerComponent>();
	this->m_pStateManagerComponent = m_pEntity->GetComponent<UnitStateManagerComponent>();

	//Change unit stance accoriding to run input
	if (m_pStateManagerComponent) {
		if (run) {
			m_pStateManagerComponent->SetStance(EUnitStance::RUNNING);
		}
		if (!run && m_pStateManagerComponent->GetStance() == EUnitStance::RUNNING) {
			m_pStateManagerComponent->SetStance(EUnitStance::WALKING);
		}
	}
}

void MoveAction::Execute()
{
	if (!m_pAiControllerComponent) {
		CryLog("MoveAction : (Cancel) AiController is null");
		bIsDone = true;
		return;
	}

	if (m_pAiControllerComponent) {
		m_pAiControllerComponent->MoveTo(m_movePosition, bRun);

		if (!m_pAttackerComponent || !m_pAttackerComponent->IsAttacking()) {
			m_pAiControllerComponent->LookAtWalkDirection();
		}
	}
}

void MoveAction::Cancel()
{
	if (!m_pAiControllerComponent) {
		CryLog("MoveAction : (Cancel) AiController is null");
		bIsDone = true;
		return;
	}

	m_pAiControllerComponent->StopMoving();
	bIsDone = true;
}

bool MoveAction::IsDone()
{
	if (!bIsDone) {
		bIsDone = m_pEntity->GetWorldPos().GetDistance(m_movePosition) <= 1;
	}
	return bIsDone;
}
