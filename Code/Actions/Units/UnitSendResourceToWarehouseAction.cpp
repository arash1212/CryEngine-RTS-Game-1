#include "StdAfx.h"
#include "UnitSendResourceToWarehouseAction.h"
#include "GamePlugin.h"

#include <Utils/EntityUtils.h>

#include <Components/BaseBuilding/Building.h>
#include <CryEntitySystem/IEntitySystem.h>
#include <Components/Selectables/ResourceStorage.h>
#include <Components/Selectables/Engineer.h>
#include <Components/Controller/AIController.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Selectables/UnitAnimation.h>
#include <Components/Managers/ResourceManager.h>
#include <Components/Selectables/ResourceCollector.h>

UnitSendResourceToWarehouseAction::UnitSendResourceToWarehouseAction(IEntity* entity, IEntity* warehouseEntity)
{
	this->m_pEntity = entity;
	this->m_pWarehouseEntity = warehouseEntity;
	this->m_pAiControllerComponent = m_pEntity->GetComponent<AIControllerComponent>();
	this->m_pStateManagerComponent = m_pEntity->GetComponent<UnitStateManagerComponent>();
	this->m_pAnimationComponent = m_pEntity->GetComponent<UnitAnimationComponent>();
	this->m_pResourceCollectorComponent = m_pEntity->GetComponent<ResourceCollectorComponent>();
}

void UnitSendResourceToWarehouseAction::Execute()
{
	if (!m_pWarehouseEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitSendResourceToWarehouseAction : (Execute) WarehouseEntity entity cannot be null.Action canceled.");
		Cancel();
		return;
	}
	if (!m_pResourceCollectorComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UnitSendResourceToWarehouseAction : (Execute) m_pResourceCollectorComponent not found on entity.Action canceled.");
		Cancel();
		return;
	}


	//If Can't collect resources anymore
	if (m_pResourceCollectorComponent->GetAmountOfResourceCollected() > 0) {
		Vec3 warehouseExitPoint = m_pWarehouseEntity->GetComponent<BuildingComponent>()->GetExitPoint();
		//Move closer to warehouse if it's not close
		f32 distanceToWareHouse = EntityUtils::GetDistance(m_pEntity->GetWorldPos(), warehouseExitPoint, nullptr);
		if (m_pWarehouseEntity && distanceToWareHouse > 1) {
			this->m_pAiControllerComponent->MoveTo(warehouseExitPoint, false);
			this->m_pAiControllerComponent->LookAtWalkDirection();
		}
		//Deliver Resource to Warehouse
		else {
			this->m_pAiControllerComponent->StopMoving();
			this->m_pAiControllerComponent->LookAt(m_pWarehouseEntity->GetWorldPos());
			this->m_pResourceCollectorComponent->SendResourceToWareHouse();
			bIsDone = true;
		}
	}
	else {
		bIsDone = true;
	}
}

void UnitSendResourceToWarehouseAction::Cancel()
{
	this->m_pAiControllerComponent->StopMoving();
	bIsDone = true;
}

bool UnitSendResourceToWarehouseAction::IsDone()
{
	return bIsDone;
}