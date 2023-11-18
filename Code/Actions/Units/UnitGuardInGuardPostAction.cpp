#include "StdAfx.h"
#include "UnitGuardInGuardPostAction.h"
#include "GamePlugin.h"

#include <Utils/EntityUtils.h>
#include <Components/BaseBuilding/Building.h>
#include <Components/Selectables/GuardPost.h>
#include <Components/Selectables/Guard.h>

#include <Components/Controller/AIController.h>
#include <Components/Managers/UnitStateManager.h>

UnitGuardInGuardPostAction::UnitGuardInGuardPostAction(IEntity* entity, IEntity* workplace)
{
	this->m_pEntity = entity;
	this->m_pGuardPostEntity = workplace;
	this->m_pAiControllerComponent = m_pEntity->GetComponent<AIControllerComponent>();
	this->m_pStateManagerComponent = m_pEntity->GetComponent<UnitStateManagerComponent>();
	this->m_pGuardPostComponent = workplace->GetOrCreateComponent<GuardPostComponent>();
	this->m_pGuardComponent = m_pEntity->GetComponent<GuardComponent>();
}

void UnitGuardInGuardPostAction::Execute()
{
	if (!m_pGuardPostEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitGuardInGuardPostAction : (Execute) m_pGuardPostEntity is null");
		bIsDone = true;
		return;
	}
	if (!m_pAiControllerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitGuardInGuardPostAction : (Execute) AiController is null");
		bIsDone = true;
		return;
	}
	if (!m_pGuardPostComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitGuardInGuardPostAction : (Execute) m_pGuardPostComponent is null");
		bIsDone = true;
		return;
	}
	if (!m_pGuardComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitGuardInGuardPostAction : (Execute) m_pGuardComponent is null");
		bIsDone = true;
		return;
	}

	if (!isAssignedToWorkplace) {
		int32 assignmentResult = m_pGuardPostComponent->AssignGuardToPlace(m_pEntity);
		if (assignmentResult == -1) {
			CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitGuardInGuardPostAction : (Execute) GaurdPost is full");
			bIsDone = true;
			return;
		}

		m_pGuardComponent->AssignGuardPost(m_pGuardPostEntity);
		isAssignedToWorkplace = true;
		nGuardIndex = assignmentResult;
	}
}

void UnitGuardInGuardPostAction::Cancel()
{
	if (!m_pAiControllerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitGuardInGuardPostAction : (Cancel) AiController is null");
		bIsDone = true;
		return;
	}

	m_pAiControllerComponent->StopMoving();
	m_pGuardComponent->CancelAssignedGuardPost();
	if (nGuardIndex != -1) {
		m_pGuardPostComponent->RemovedGuardFromWorkplace(nGuardIndex);
	}

	if (m_pGuardComponent->HasEnteredGuardPost()) {
		m_pEntity->SetPos(m_pGuardPostEntity->GetComponent<BuildingComponent>()->GetExitPoint());
	}
	bIsDone = true;
}

bool UnitGuardInGuardPostAction::IsDone()
{
	return bIsDone;
}

bool UnitGuardInGuardPostAction::CanBeSkipped()
{
	return true;
}
