#include "StdAfx.h"
#include "UnitBuildAction.h"
#include "GamePlugin.h"

#include <Utils/MathUtils.h>
#include <Utils/EntityUtils.h>

#include <CryGame/IGameFramework.h>

#include <Components/BaseBuilding/Building.h>
#include <Components/Selectables/Engineer.h>
#include <Components/Controller/AIController.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Selectables/UnitAnimation.h>
#include <Components/Managers/ResourceManager.h>
#include <Components/Selectables/Cost.h>

UnitBuildAction::UnitBuildAction(IEntity* entity, IEntity* buildingEntity)
{
	this->m_pEntity = entity;
	this->m_pBuildingEntity = buildingEntity;
	this->m_pEngineerComponent = m_pEntity->GetComponent<EngineerComponent>();
	this->m_pAiControllerComponent = m_pEntity->GetComponent<AIControllerComponent>();
	this->m_pStateManagerComponent = m_pEntity->GetComponent<UnitStateManagerComponent>();
	this->m_pBuildingComponent = buildingEntity->GetComponent<BuildingComponent>();
	this->m_pAnimationComponent = m_pEntity->GetComponent<UnitAnimationComponent>();

	this->m_pEngineerComponent->AssignBuilding(buildingEntity);
	this->m_pBuildingComponent->AddBuilder(m_pEntity);
	m_builtTimePassed = m_pEngineerComponent->GetEngineerInfo().m_timeBetweenBuilds;

	m_movePosition = EntityUtils::GetClosetPointOnMeshBorder(m_pEntity->GetWorldPos(), m_pBuildingEntity);

	if (m_pBuildingComponent->GetBuilders().size() > 5) {
		Cancel();
	}
}

void UnitBuildAction::Execute()
{
	if (!m_pBuildingEntity || m_pBuildingEntity->IsGarbage()) {
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

	f32 distanceToBuilding = EntityUtils::GetDistance(m_pEntity->GetWorldPos(), m_pBuildingEntity->GetWorldPos(), m_pBuildingEntity);
	if (distanceToBuilding <= m_pEngineerComponent->GetEngineerInfo().m_maxBuildDistance) {
		this->m_pAiControllerComponent->StopMoving();
		this->m_pBuildingComponent->Build();
		this->m_pAiControllerComponent->LookAt(m_pBuildingEntity->GetWorldPos());
		this->m_pAnimationComponent->PlayRandomAttackAnimation();
		this->m_builtTimePassed = 0;
	}
	else {
		m_movePosition = GetClosestPointAvailableCloseToBuilding();
		this->m_pAiControllerComponent->MoveTo(m_movePosition, true);
		this->m_pAiControllerComponent->LookAtWalkDirection();
	}
}

void UnitBuildAction::Cancel()
{
	this->m_pEngineerComponent->CancelBuildingAssigned();
	this->m_pAiControllerComponent->StopMoving();
	bIsDone = true;
	m_pBuildingComponent->RemoveBuilder(m_pEntity);
}

bool UnitBuildAction::IsDone()
{
	return this->m_pBuildingComponent->IsBuilt() || bIsDone;
}

f32 UnitBuildAction::GetProgressAmount()
{
	return this->m_pBuildingComponent->GetCurrentBuildAmount();
}

f32 UnitBuildAction::GetMaxProgressAmount()
{
	return this->m_pBuildingComponent->GetBuildingInfos().m_maxBuildAmount;
}

bool UnitBuildAction::IsMoveToPointAvailable()
{
	for (IEntity* builder : m_pBuildingComponent->GetBuilders()) {
		if (builder == m_pEntity || !builder) {
			continue;
		}
		f32 distanceToBuilding = m_movePosition.GetDistance(m_pBuildingEntity->GetWorldPos());
		if (distanceToBuilding > m_pEngineerComponent->GetEngineerInfo().m_maxBuildDistance) {
			return false;
		}

		f32 distanceToBuilder = m_movePosition.GetDistance(builder->GetWorldPos());
		if (distanceToBuilder <= 0.5f) {
			return false;
		}
	}
	return true;
}

Vec3 UnitBuildAction::GetClosestPointAvailableCloseToBuilding()
{
	if (IsMoveToPointAvailable() && m_pAiControllerComponent->IsDestinationReachable(m_movePosition)) {
		return m_movePosition;
	}
	return EntityUtils::GetRandomPointOnMeshBorder(m_pBuildingEntity);
}