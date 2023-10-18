#include "StdAfx.h"
#include "TrainSoldier1Action.h"
#include "GamePlugin.h"

#include <CryEntitySystem/IEntitySystem.h>

#include <Components/Selectables/Attacker.h>
#include <Components/Controller/AIController.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Selectables/Units/Soldier1Unit.h>
#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/ResourceManager.h>
#include <Components/Selectables/Cost.h>

TrainSoldier1Action::TrainSoldier1Action(IEntity* entity, Vec3 exitPos)
{
	this->m_pEntity = entity;
	this->m_exitPosition = exitPos;
}

void TrainSoldier1Action::Execute()
{
	if (m_exitPosition == ZERO) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "TrainSoldier1Action:(Execute) exitPos cannot be ZERO !");
		Cancel();
		return;
	}

	//Timer
	if (m_ProcessTimePassesd < m_timeBetweenProcess) {
		m_ProcessTimePassesd += 0.5f * gEnv->pTimer->GetFrameTime();
	}
	else {
		m_processAmount += 0.2f;
		m_ProcessTimePassesd = 0;
	}

	if (m_processAmount < 1) {
		return;
	}
	OwnerInfoComponent* onwerInfo = m_pEntity->GetComponent<OwnerInfoComponent>();
	if (!onwerInfo) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "TrainSoldier1Action:(Execute) onwerInfo component does not exist on the building. train action cancelled.");
		Cancel();
		return;
	}

	SEntitySpawnParams entitySpawnParams;
	entitySpawnParams.vPosition = m_exitPosition;
	entitySpawnParams.qRotation = IDENTITY;
	IEntity* spawnedEntity = gEnv->pEntitySystem->SpawnEntity(entitySpawnParams);
	spawnedEntity->GetOrCreateComponent<Soldier1UnitComponent>();
	spawnedEntity->GetComponent<OwnerInfoComponent>()->SetOwner(onwerInfo->GetOwner());
	bIsDone = true;
}

void TrainSoldier1Action::Cancel()
{
	bIsDone = true;
}

bool TrainSoldier1Action::IsDone()
{
	return bIsDone;
}