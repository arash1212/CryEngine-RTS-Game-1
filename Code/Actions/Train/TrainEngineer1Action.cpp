#include "StdAfx.h"
#include "TrainEngineer1Action.h"
#include "GamePlugin.h"

#include <CryEntitySystem/IEntitySystem.h>
#include <Utils/EntityUtils.h>

#include <UIItems/InfoPanel/Items/UIActionInfoPanelItem.h>

#include <Components/Selectables/Attacker.h>
#include <Components/Controller/AIController.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Selectables/Units/Engineer1Unit.h>
#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/ResourceManager.h>
#include <Components/Selectables/Cost.h>

TrainEngineer1Action::TrainEngineer1Action(IEntity* entity, Vec3 exitPos)
{
	this->m_pEntity = entity;
	this->m_exitPosition = exitPos;
}

void TrainEngineer1Action::Execute()
{
	if (m_exitPosition == ZERO) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "BuildingTrainEngineer1Action:(Execute) exitPos cannot be ZERO !");
		Cancel();
		return;
	}

	//Timer
	if (m_ProcessTimePassesd < m_timeBetweenProcess) {
		m_ProcessTimePassesd += 0.5f * gEnv->pTimer->GetFrameTime();
	}
	else {
		m_processAmount += 1.f;
		m_ProcessTimePassesd = 0;
	}

	if (m_processAmount < GetMaxProgressAmount()) {
		return;
	}
	OwnerInfoComponent* pOnwerInfoComponent = m_pEntity->GetComponent<OwnerInfoComponent>();
	if (!pOnwerInfoComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "BuildingTrainEngineer1Action:(Execute) onwerInfo component does not exist on the building. train action cancelled.");
		Cancel();
		return;
	}
	IEntity* pOwnerEntity = pOnwerInfoComponent->GetOwner();
	if (!pOwnerEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "BuildingTrainEngineer1Action:(Execute) ownerEntity is null.");
		Cancel();
		return;
	}

	IEntity* spawnedEntity = EntityUtils::SpawnEntity(m_exitPosition, IDENTITY, pOwnerEntity);
	spawnedEntity->GetOrCreateComponent<Engineer1UnitComponent>();
	bIsDone = true;
}

void TrainEngineer1Action::Cancel()
{
	OwnerInfoComponent* pOnwerInfoComponent = m_pEntity->GetComponent<OwnerInfoComponent>();
	if (!pOnwerInfoComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "TrainEngineer1Action:(Cancel) onwerInfo component does not exist on the building. train action cancelled.");
		Cancel();
		return;
	}
	IEntity* pOwnerEntity = pOnwerInfoComponent->GetOwner();
	if (!pOwnerEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "TrainEngineer1Action:(Cancel) ownerEntity is null.");
		Cancel();
		return;
	}
	ResourceManagerComponent* resouceManager = pOwnerEntity->GetComponent<ResourceManagerComponent>();
	if (!resouceManager) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "TrainEngineer1Action : (Cancel) resoucwManager is null !");
		return;
	}

	resouceManager->RefundResources(Engineer1UnitComponent::GetDescription().price);
	bIsDone = true;
}

bool TrainEngineer1Action::IsDone()
{
	return bIsDone;
}

f32 TrainEngineer1Action::GetProgressAmount()
{
	return crymath::floor(m_processAmount);
}

IBaseInfoPanelUIItem* TrainEngineer1Action::GetInfoPanelItem()
{
	return new UIActionInfoPanelItem(m_pEntity, Engineer1UnitComponent::GetDescription().sIcon, this);
}

