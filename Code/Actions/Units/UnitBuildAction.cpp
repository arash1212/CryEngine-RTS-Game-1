#include "StdAfx.h"
#include "UnitBuildAction.h"
#include "GamePlugin.h"

#include <Components/BaseBuilding/Building.h>
#include <Components/Selectables/Engineer.h>
#include <Components/Controller/AIController.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Selectables/UnitAnimation.h>
#include <Components/Managers/ResourceManager.h>
#include <Components/Selectables/Cost.h>

UnitBuildAction::UnitBuildAction(IEntity* entity, IEntity* buildiingEntity)
{
	this->m_pEntity = entity;
	this->m_pBuildingEntity = buildiingEntity;
	this->m_pEngineerComponent = m_pEntity->GetComponent<EngineerComponent>();
	this->m_pAiControllerComponent = m_pEntity->GetComponent<AIControllerComponent>();
	this->m_pStateManagerComponent = m_pEntity->GetComponent<UnitStateManagerComponent>();
	this->m_pBuildingComponent = buildiingEntity->GetComponent<BuildingComponent>();
	this->m_pAnimationComponent = m_pEntity->GetComponent<UnitAnimationComponent>();

	this->m_pEngineerComponent->AssignBuilding(buildiingEntity);
	m_builtTimePassed = m_pEngineerComponent->GetEngineerInfo().m_timeBetweenBuilds;
}

void UnitBuildAction::Execute()
{
	if (!m_pBuildingEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitBuildAction : (Execute) Building entity cannot be null.Action canceled.");
		Cancel();
		return;
	}
	if (!m_pEngineerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitBuildAction : (Execute) Selected Unit is not an Engineer.Action canceled.");
		Cancel();
		return;
	}
	if (this->m_pBuildingComponent->IsBuilt()) {
		bIsDone = true;
		return;
	}

	//Timer
	if (m_builtTimePassed < m_pEngineerComponent->GetEngineerInfo().m_timeBetweenBuilds) {
		m_builtTimePassed += 0.5f * gEnv->pTimer->GetFrameTime();
		return;
	}

	f32 distanceToBuilding = m_pEntity->GetWorldPos().GetDistance(m_pBuildingEntity->GetWorldPos());
	if (distanceToBuilding <= m_pEngineerComponent->GetEngineerInfo().m_maxBuildDistance) {
		this->m_pAiControllerComponent->StopMoving();
		this->m_pBuildingComponent->Build();
		this->m_pAiControllerComponent->LookAt(m_pBuildingEntity->GetWorldPos());
		this->m_pAnimationComponent->PlayRandomAttackAnimation();
		this->m_builtTimePassed = 0;
	}
	else {
		this->m_pAiControllerComponent->MoveTo(m_pBuildingEntity->GetWorldPos(), true);
		this->m_pAiControllerComponent->LookAtWalkDirection();
	}
}

void UnitBuildAction::Cancel()
{
	this->m_pEngineerComponent->CancelBuildingAssigned();
	this->m_pAiControllerComponent->StopMoving();
	bIsDone = true;
}

bool UnitBuildAction::IsDone()
{
	return this->m_pBuildingComponent->IsBuilt() || bIsDone;
}