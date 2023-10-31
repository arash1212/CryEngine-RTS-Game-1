#include "StdAfx.h"
#include "UnitCollectResourceAction.h"
#include "GamePlugin.h"

#include <Utils/EntityUtils.h>

#include <CryEntitySystem/IEntitySystem.h>
#include <Components/Selectables/ResourceStorage.h>
#include <Components/Resources/Resource.h>
#include <Components/Selectables/Engineer.h>
#include <Components/Controller/AIController.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Selectables/UnitAnimation.h>
#include <Components/Managers/ResourceManager.h>
#include <Components/Selectables/ResourceCollector.h>

UnitCollectResourceAction::UnitCollectResourceAction(IEntity* entity, IEntity* resourceEntity)
{
	this->m_pEntity = entity;
	this->m_pResourceEntity = resourceEntity;
	this->m_pEngineerComponent = m_pEntity->GetComponent<EngineerComponent>();
	this->m_pAiControllerComponent = m_pEntity->GetComponent<AIControllerComponent>();
	this->m_pStateManagerComponent = m_pEntity->GetComponent<UnitStateManagerComponent>();
	this->m_pResourceComponent = resourceEntity->GetComponent<ResourceComponent>();
	this->m_pAnimationComponent = m_pEntity->GetComponent<UnitAnimationComponent>();
	this->m_pResourceCollectorComponent = m_pEntity->GetComponent<ResourceCollectorComponent>();

	m_builtTimePassed = this->m_pEngineerComponent->GetEngineerInfo().m_timeBetweenBuilds;
}


void UnitCollectResourceAction::Execute()
{
	if (!m_pResourceEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitCollectResourceAction : (Execute) Resource entity cannot be null.Action canceled.");
		Cancel();
		return;
	}
	if (!m_pEngineerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitCollectResourceAction : (Execute) Selected Unit is not an Engineer.Action canceled.");
		Cancel();
		return;
	}
	if (!m_pResourceComponent) {
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
	if (m_builtTimePassed < m_pEngineerComponent->GetEngineerInfo().m_timeBetweenBuilds) {
		m_builtTimePassed += 0.5f * gEnv->pTimer->GetFrameTime();
		return;
	}
	
	Vec3 collectingLocationPos = ZERO;
	f32 distanceToResource = 100000.f;

	//*********************************Collecting location
	if (m_pResourceComponent->HasCollectingLocation()) {
		collectingLocationPos = m_pResourceEntity->GetComponent<ResourceComponent>()->GetCollectingLocation();
		distanceToResource = EntityUtils::GetDistance(m_pEntity->GetWorldPos(), m_pResourceComponent->GetCollectingLocation(), nullptr);
	}
	else {
		distanceToResource = EntityUtils::GetDistance(m_pEntity->GetWorldPos(), EntityUtils::GetClosetPointOnMeshBorder(m_pEntity->GetWorldPos(), m_pResourceEntity), nullptr);
	}

	if (distanceToResource <= m_pEngineerComponent->GetEngineerInfo().m_maxBuildDistance && m_pResourceCollectorComponent->CanCollectResource()) {
		if (m_pResourceComponent->IsSingleUse() && m_pResourceComponent->IsInUse() && m_pResourceComponent->GetCurrentCollector()->GetId() != m_pEntity->GetId()) {
			return;
		}

		if (this->m_pResourceCollectorComponent->GetCurrentResourceType() != m_pResourceComponent->GetType()) {
			this->m_pResourceCollectorComponent->SetCurrentResourceType(m_pResourceComponent->GetType());
			this->m_pResourceCollectorComponent->EmptyResources();
		}

		this->m_pAiControllerComponent->StopMoving();

		if (m_pResourceComponent->HasCollectingLocation()) {
			this->m_pAiControllerComponent->LookAt(collectingLocationPos);
		}
		else {
			this->m_pAiControllerComponent->LookAt(m_pResourceEntity->GetWorldPos());
		}

		this->m_pAnimationComponent->PlayRandomAttackAnimation();
		this->m_builtTimePassed = 0;
		this->m_pResourceCollectorComponent->AddResource(5);

		this->m_pResourceComponent->SetIsInUse(true);
		this->m_pResourceComponent->SetCurrentCollector(m_pEntity);
	}
	else if (distanceToResource > m_pEngineerComponent->GetEngineerInfo().m_maxBuildDistance && m_pResourceCollectorComponent->CanCollectResource()) {
		if (!m_pResourceComponent->HasCollectingLocation() || m_pResourceComponent->HasCollectingLocation()&& m_pResourceComponent->IsSingleUse() && m_pResourceComponent->IsInUse() && m_pResourceComponent->GetCurrentCollector()->GetId() != m_pEntity->GetId()) {
			this->m_pAiControllerComponent->MoveTo(EntityUtils::GetClosetPointOnMeshBorder(m_pEntity->GetWorldPos(), m_pResourceEntity), true);
			this->m_pAiControllerComponent->LookAtWalkDirection();
		}
		else {
			this->m_pAiControllerComponent->MoveTo(collectingLocationPos, true);
			this->m_pAiControllerComponent->LookAtWalkDirection();
		}
	}

	//If Can't collect resources anymore
	else if (!m_pResourceCollectorComponent->CanCollectResource()) {
		if (m_pResourceComponent->IsInUse() && m_pResourceComponent->GetCurrentCollector() == m_pEntity) {
			this->m_pResourceComponent->SetIsInUse(false);
			this->m_pResourceComponent->SetCurrentCollector(nullptr);
		}

		if (!m_pWarehouseEntity) {
			m_pWarehouseEntity = EntityUtils::FindClosestWarehouse(m_pEntity);
			this->m_builtTimePassed = 0.2f;
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
			this->m_pAiControllerComponent->LookAt(EntityUtils::GetClosetPointOnMeshBorder(m_pEntity->GetWorldPos(), m_pWarehouseEntity));
			this->m_pResourceCollectorComponent->SendResourceToWareHouse();
		}
	}
}

void UnitCollectResourceAction::Cancel()
{
	if (m_pResourceComponent->GetCurrentCollector() && m_pResourceComponent->GetCurrentCollector()->GetId() != m_pEntity->GetId()) {
		this->m_pResourceComponent->SetIsInUse(false);
		this->m_pResourceComponent->SetCurrentCollector(nullptr);
	}
	this->m_pEngineerComponent->CancelBuildingAssigned();
	this->m_pAiControllerComponent->StopMoving();
	bIsDone = true;
}

bool UnitCollectResourceAction::IsDone()
{
	return bIsDone;
}