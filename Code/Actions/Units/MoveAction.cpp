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
}

void MoveAction::Execute()
{
	AIControllerComponent* controller = m_pEntity->GetComponent<AIControllerComponent>();
	AttackerComponent* attacker = m_pEntity->GetComponent<AttackerComponent>();
	//UnitStateManagerComponent* stateManager = m_pEntity->GetComponent<UnitStateManagerComponent>();
	if (controller) {
		controller->MoveTo(m_movePosition, bRun);

		if (!attacker || !attacker->IsAttacking()) {
			controller->LookAtWalkDirection();
		}
	}
}

void MoveAction::Cancel()
{
	AIControllerComponent* controller = m_pEntity->GetComponent<AIControllerComponent>();
	if (controller) {
		controller->MoveTo(m_pEntity->GetWorldPos(), false);
		bIsDone = true;
	}
}

bool MoveAction::IsDone()
{
	if (!bIsDone) {
		bIsDone = m_pEntity->GetWorldPos().GetDistance(m_movePosition) <= 1;
	}
	return bIsDone;
}
