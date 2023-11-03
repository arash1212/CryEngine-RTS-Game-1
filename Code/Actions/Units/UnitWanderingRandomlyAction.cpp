#include "StdAfx.h"
#include "UnitWanderingRandomlyAction.h"
#include "GamePlugin.h"

#include <Utils/EntityUtils.h>

#include <Components/Selectables/Attacker.h>
#include <Components/Controller/AIController.h>
#include <Components/Managers/UnitStateManager.h>

UnitWanderingRandomlyAction::UnitWanderingRandomlyAction(IEntity* entity, IEntity* around, bool run)
{
	this->m_pEntity = entity;
	this->m_pAround = around;
	this->bRun = run;
	this->m_pAttackerComponent = m_pEntity->GetComponent<AttackerComponent>();
	this->m_pAiControllerComponent = m_pEntity->GetComponent<AIControllerComponent>();
	this->m_pStateManagerComponent = m_pEntity->GetComponent<UnitStateManagerComponent>();

	//Change unit stance accoriding to run input
	if (m_pStateManagerComponent) {
		if (run) {
			//m_pStateManagerComponent->SetStance(EUnitStance::RUNNING);
		}
		if (!run && m_pStateManagerComponent->GetStance() == EUnitStance::RUNNING) {
			//m_pStateManagerComponent->SetStance(EUnitStance::WALKING);
		}
	}
}

void UnitWanderingRandomlyAction::Execute()
{
	if (!m_pAiControllerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitWanderingRandomlyAction : (Cancel) AiController is null");
		bIsDone = true;
		return;
	}
	if (m_pAttackerComponent->IsAttacking()) {
		//m_pStateManagerComponent->SetStance(EUnitStance::RUNNING);
		Cancel();
		return;
	}

	//Find new point if lastPoint is Closer than 1
	f32 distanceToMoveToPos = m_pEntity->GetWorldPos().GetDistance(m_movePosition);
	if (m_movePosition == ZERO || distanceToMoveToPos < 1) {
		m_movePosition = m_pAiControllerComponent->GetRandomPointOnNavmesh(20, m_pAround);
	}

	if (m_pAiControllerComponent) {
		m_pAiControllerComponent->MoveTo(m_movePosition, bRun);
		m_pAiControllerComponent->LookAtWalkDirection();
	}
}

void UnitWanderingRandomlyAction::Cancel()
{
	if (!m_pAiControllerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitWanderingRandomlyAction : (Cancel) AiController is null");
		bIsDone = true;
		return;
	}

	m_pAiControllerComponent->StopMoving();
	bIsDone = true;
}

bool UnitWanderingRandomlyAction::IsDone()
{
	if (!bIsDone) {
		//bIsDone = m_pAttackerComponent->IsAttacking();
	}
	return bIsDone;
}

bool UnitWanderingRandomlyAction::CanBeSkipped()
{
	return true;
}
