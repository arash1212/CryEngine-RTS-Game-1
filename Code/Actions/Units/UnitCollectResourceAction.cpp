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

IEntity* UnitCollectResourceAction::FindClosestWarehouse()
{
	IEntityItPtr entityItPtr = gEnv->pEntitySystem->GetEntityIterator();
	entityItPtr->MoveFirst();
	while (!entityItPtr->IsEnd()) {
		IEntity* entity = entityItPtr->Next();
		if (entity) {
			ResourceStorageComponent* resourceStorage = entity->GetComponent<ResourceStorageComponent>();
			if (resourceStorage) {
				return entity;
			}
		}
	}
	return nullptr;
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
	

	Vec3 collectingLocationPos = m_pResourceEntity->GetComponent<ResourceComponent>()->GetCollectingLocation();
	f32 distanceToResource = EntityUtils::GetDistance(m_pEntity->GetWorldPos(), collectingLocationPos, nullptr);
	if (distanceToResource <= m_pEngineerComponent->GetEngineerInfo().m_maxBuildDistance && m_pResourceCollectorComponent->CanCollectResource()) {

		if (this->m_pResourceCollectorComponent->GetCurrentResourceType() != m_pResourceComponent->GetType()) {
			this->m_pResourceCollectorComponent->SetCurrentResourceType(m_pResourceComponent->GetType());
			this->m_pResourceCollectorComponent->EmptyResources();
		}

		this->m_pAiControllerComponent->StopMoving();
		this->m_pAiControllerComponent->LookAt(collectingLocationPos);
		this->m_pAnimationComponent->PlayRandomAttackAnimation();
		this->m_builtTimePassed = 0;
		this->m_pResourceCollectorComponent->AddResource(5);
	}
	else if (distanceToResource > m_pEngineerComponent->GetEngineerInfo().m_maxBuildDistance && m_pResourceCollectorComponent->CanCollectResource()) {
		this->m_pAiControllerComponent->MoveTo(collectingLocationPos, true);
		this->m_pAiControllerComponent->LookAtWalkDirection();
	}

	//If Can't collect resources anymore
	else if (!m_pResourceCollectorComponent->CanCollectResource()) {
		if (!m_pWarehouseEntity) {
			m_pWarehouseEntity = FindClosestWarehouse();
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
			this->m_pAiControllerComponent->LookAt(m_pWarehouseEntity->GetWorldPos());
			this->m_pResourceCollectorComponent->SendResourceToWareHouse();
		}
	}
}

void UnitCollectResourceAction::Cancel()
{
	this->m_pEngineerComponent->CancelBuildingAssigned();
	this->m_pAiControllerComponent->StopMoving();
	bIsDone = true;
}

bool UnitCollectResourceAction::IsDone()
{
	return bIsDone;
}