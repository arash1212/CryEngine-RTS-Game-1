#include "StdAfx.h"
#include "UnitCollectResourceAction.h"
#include "GamePlugin.h"

#include <Utils/EntityUtils.h>

#include <CryEntitySystem/IEntitySystem.h>
#include <Components/Selectables/ResourceStorage.h>
#include <Components/ResourcePoints/BaseResourcePoint.h>
#include <Components/Selectables/Engineer.h>
#include <Components/Controller/AIController.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Selectables/UnitAnimation.h>
#include <Components/Managers/ResourceManager.h>
#include <Components/Selectables/ResourceCollector.h>

UnitCollectResourceAction::UnitCollectResourceAction(IEntity* entity, IEntity* resourceEntity)
{
	this->m_pEntity = entity;
	this->m_pResourcePointEntity = resourceEntity;
	this->m_pEngineerComponent = m_pEntity->GetComponent<EngineerComponent>();
	this->m_pAiControllerComponent = m_pEntity->GetComponent<AIControllerComponent>();
	this->m_pStateManagerComponent = m_pEntity->GetComponent<UnitStateManagerComponent>();
	this->m_pResourcePointComponent = resourceEntity->GetComponent<BaseResourcePointComponent>();
	this->m_pAnimationComponent = m_pEntity->GetComponent<UnitAnimationComponent>();
	this->m_pResourceCollectorComponent = m_pEntity->GetComponent<ResourceCollectorComponent>();

	this->m_pResourcePointComponent->AddCollector(m_pEntity);

	fCollectingTimePassed = this->m_pEngineerComponent->GetEngineerInfo().m_timeBetweenCollecting;

	m_movePosition = EntityUtils::GetClosetPointOnMeshBorder(m_pEntity->GetWorldPos(), m_pResourcePointEntity);
}


void UnitCollectResourceAction::Execute()
{
	if (!m_pResourcePointEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitCollectResourceAction : (Execute) Resource entity cannot be null.Action canceled.");
		Cancel();
		return;
	}
	if (!m_pEngineerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitCollectResourceAction : (Execute) Selected Unit is not an Engineer.Action canceled.");
		Cancel();
		return;
	}
	if (!m_pResourcePointComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitCollectResourceAction : (Execute) resourceComponent not found on selected resource.Action canceled.");
		Cancel();
		return;
	}
	if (!m_pResourceCollectorComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitCollectResourceAction : (Execute) m_pResourceCollectorComponent not found on entity.Action canceled.");
		Cancel();
		return;
	}

	//Timer
	if (fCollectingTimePassed < m_pEngineerComponent->GetEngineerInfo().m_timeBetweenCollecting) {
		fCollectingTimePassed += 0.5f * gEnv->pTimer->GetFrameTime();
		return;
	}
	
	Vec3 collectingLocationPos = ZERO;
	f32 distanceToResource = 100000.f;

	//*********************************Collecting location
	if (m_pResourcePointComponent->HasCollectingLocation()) {
		collectingLocationPos = m_pResourcePointEntity->GetComponent<BaseResourcePointComponent>()->GetCollectingLocation();
		distanceToResource = EntityUtils::GetDistance(m_pEntity->GetWorldPos(), m_pResourcePointComponent->GetCollectingLocation(), nullptr);
	}
	else {
		distanceToResource = EntityUtils::GetDistance(m_pEntity->GetWorldPos(), EntityUtils::GetClosetPointOnMeshBorder(m_pEntity->GetWorldPos(), m_pResourcePointEntity), nullptr);
	}

	if (distanceToResource <= m_pEngineerComponent->GetEngineerInfo().m_maxBuildDistance && m_pResourceCollectorComponent->CanCollectResource(nCollectedAmount)) {
		if (m_pResourcePointComponent->IsSingleUse() && m_pResourcePointComponent->IsInUse() && m_pResourcePointComponent->GetCurrentCollector()->GetId() != m_pEntity->GetId()) {
			return;
		}

		if (this->m_pResourceCollectorComponent->GetCurrentResourceType() != m_pResourcePointComponent->GetType()) {
			this->m_pResourceCollectorComponent->SetCurrentResourceType(m_pResourcePointComponent->GetType());
			this->m_pResourceCollectorComponent->EmptyResources();
		}

		this->m_pAiControllerComponent->StopMoving();

		if (m_pResourcePointComponent->HasCollectingLocation()) {
			this->m_pAiControllerComponent->LookAt(collectingLocationPos);
		}
		else {
			this->m_pAiControllerComponent->LookAt(m_pResourcePointEntity->GetWorldPos());
		}

		this->m_pAnimationComponent->PlayRandomAttackAnimation();
		this->fCollectingTimePassed = 0;
		//old
		//this->m_pResourceCollectorComponent->AddResource(1);
		this->nCollectedAmount += 1;

		this->m_pResourcePointComponent->SetIsInUse(true);
		this->m_pResourcePointComponent->SetCurrentCollector(m_pEntity);
	}
	else if (distanceToResource > m_pEngineerComponent->GetEngineerInfo().m_maxBuildDistance && m_pResourceCollectorComponent->CanCollectResource(nCollectedAmount)) {
		if (!m_pResourcePointComponent->HasCollectingLocation() || m_pResourcePointComponent->HasCollectingLocation()&& m_pResourcePointComponent->IsSingleUse() && m_pResourcePointComponent->IsInUse() && m_pResourcePointComponent->GetCurrentCollector()->GetId() != m_pEntity->GetId()) {
			this->m_pAiControllerComponent->MoveTo(GetClosestPointAvailableCloseToBuilding(), true);
			this->m_pAiControllerComponent->LookAtWalkDirection();
		}
		else {
			this->m_pAiControllerComponent->MoveTo(collectingLocationPos, true);
			this->m_pAiControllerComponent->LookAtWalkDirection();
		}
	}

	//If Can't collect resources anymore
	else if (!m_pResourceCollectorComponent->CanCollectResource(nCollectedAmount)) {
		if (!bResourcesAddedToCollector) {
			this->m_pResourceCollectorComponent->AddResource(nCollectedAmount);
			this->bResourcesAddedToCollector = true;
		}

		if (m_pResourcePointComponent->IsInUse() && m_pResourcePointComponent->GetCurrentCollector() == m_pEntity) {
			this->m_pResourcePointComponent->SetIsInUse(false);
			this->m_pResourcePointComponent->SetCurrentCollector(nullptr);
		}

		if (!m_pWarehouseEntity) {
			m_pWarehouseEntity = EntityUtils::FindClosestWarehouse(m_pEntity);
			this->fCollectingTimePassed = 0.2f;
			return;
		}

		Vec3 warehouseExitPoint = m_pWarehouseEntity->GetComponent<BuildingComponent>()->GetExitPoint();
		//Move closer to warehouse if it's not close
		f32 distanceToWareHouse = EntityUtils::GetDistance(m_pEntity->GetWorldPos(), warehouseExitPoint, nullptr);
		if (m_pWarehouseEntity && distanceToWareHouse > m_pEngineerComponent->GetEngineerInfo().m_maxBuildDistance) {
			this->m_pAiControllerComponent->MoveTo(warehouseExitPoint, false);
			this->m_pAiControllerComponent->LookAtWalkDirection();
		}
		//Deliver Resource to Warehouse
		else {
			this->m_pAiControllerComponent->StopMoving();
			this->m_pAiControllerComponent->LookAt(m_pEntity->GetWorldPos());
			this->m_pResourceCollectorComponent->SendResourceToWareHouse();
			this->bResourcesAddedToCollector = false;
			this->nCollectedAmount = 0;
		}
	}
}

void UnitCollectResourceAction::Cancel()
{
	if (m_pResourcePointComponent->GetCurrentCollector() && m_pResourcePointComponent->GetCurrentCollector()->GetId() != m_pEntity->GetId()) {
		this->m_pResourcePointComponent->SetIsInUse(false);
		this->m_pResourcePointComponent->SetCurrentCollector(nullptr);
	}
	this->m_pEngineerComponent->CancelBuildingAssigned();
	this->m_pAiControllerComponent->StopMoving();
	bIsDone = true;
	this->m_pResourcePointComponent->RemoveCollector(m_pEntity);
}

bool UnitCollectResourceAction::IsDone()
{
	return bIsDone;
}

bool UnitCollectResourceAction::IsMoveToPointAvailable()
{
	for (IEntity* builder : m_pResourcePointComponent->GetCollectors()) {
		if (builder == m_pEntity) {
			continue;
		}

		f32 distanceToBuilder = m_movePosition.GetDistance(builder->GetWorldPos());
		if (distanceToBuilder <= 1) {
			return false;
		}
	}
	return true;
}

Vec3 UnitCollectResourceAction::GetClosestPointAvailableCloseToBuilding()
{
	if (IsMoveToPointAvailable() && m_pAiControllerComponent->IsDestinationReachable(m_movePosition)) {
		return m_movePosition;
	}
	return EntityUtils::GetRandomPointOnMeshBorder(m_pResourcePointEntity);
}