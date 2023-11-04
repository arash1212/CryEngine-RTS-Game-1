#include "StdAfx.h"
#include "UnitWorkInWorkplaceAction.h"
#include "GamePlugin.h"

#include <Utils/EntityUtils.h>
#include <Components/BaseBuilding/Building.h>
#include <Components/Selectables/Workplace.h>
#include <Components/Selectables/Worker.h>

#include <Components/Selectables/Attacker.h>
#include <Components/Controller/AIController.h>
#include <Components/Managers/UnitStateManager.h>

UnitWorkInWorkplaceAction::UnitWorkInWorkplaceAction(IEntity* entity, IEntity* workplace)
{
	this->m_pEntity = entity;
	this->m_pWorkplaceEntity = workplace;
	this->m_pAttackerComponent = m_pEntity->GetComponent<AttackerComponent>();
	this->m_pAiControllerComponent = m_pEntity->GetComponent<AIControllerComponent>();
	this->m_pStateManagerComponent = m_pEntity->GetComponent<UnitStateManagerComponent>();
	this->m_pWorkplaceComponent = m_pWorkplaceEntity->GetComponent<WorkplaceComponent>();
	this->m_pWorkerComponent = m_pEntity->GetComponent<WorkerComponent>();
}

void UnitWorkInWorkplaceAction::Execute()
{
	if (!m_pWorkplaceEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitWorkInWorkplaceAction : (Execute) WorkplaceEntity is null");
		bIsDone = true;
		return;
	}
	if (!m_pAiControllerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitWorkInWorkplaceAction : (Execute) AiController is null");
		bIsDone = true;
		return;
	}
	if (!m_pWorkplaceComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitWorkInWorkplaceAction : (Execute) m_pWorkplaceComponent is null");
		bIsDone = true;
		return;
	}
	if (!m_pWorkerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitWorkInWorkplaceAction : (Execute) m_pWorkerComponent is null");
		bIsDone = true;
		return;
	}

	if (!isAssignedToWorkplace) {
		int32 assignmentResult = m_pWorkplaceComponent->AssignWorkerToPlace(m_pEntity);
		if (assignmentResult == -1) {
			CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitWorkInWorkplaceAction : (Execute) Workplace is full");
			bIsDone = true;
			return;
		}
		
		m_pWorkerComponent->AssignWorkplace(m_pWorkplaceEntity);
		isAssignedToWorkplace = true;
		m_workerIndex = assignmentResult;
	}
}

void UnitWorkInWorkplaceAction::Cancel()
{
	if (!m_pAiControllerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitWorkInWorkplaceAction : (Cancel) AiController is null");
		bIsDone = true;
		return;
	}

	m_pAiControllerComponent->StopMoving();
	m_pWorkerComponent->CancelAssignedWorkplace();
	if (m_workerIndex != -1) {
		m_pWorkplaceComponent->RemovedWorkerFromWorkplace(m_workerIndex);
	}
	bIsDone = true;
}

bool UnitWorkInWorkplaceAction::IsDone()
{
	return bIsDone;
}

bool UnitWorkInWorkplaceAction::CanBeSkipped()
{
	return false;
}
