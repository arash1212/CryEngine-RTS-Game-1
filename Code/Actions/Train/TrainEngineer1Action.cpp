#include "StdAfx.h"
#include "TrainEngineer1Action.h"
#include "GamePlugin.h"

#include <CryEntitySystem/IEntitySystem.h>

#include <Components/Selectables/Attacker.h>
#include <Components/Controller/AIController.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Selectables/Units/Engineer1Unit.h>

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
		m_processAmount += 0.2f;
		m_ProcessTimePassesd = 0;
	}

	if (m_processAmount < 1) {
		return;
	}

	SEntitySpawnParams entitySpawnParams;
	entitySpawnParams.vPosition = m_exitPosition;
	entitySpawnParams.qRotation = IDENTITY;
	IEntity* spawnedEntity = gEnv->pEntitySystem->SpawnEntity(entitySpawnParams);
	spawnedEntity->GetOrCreateComponent<Engineer1UnitComponent>();
	bIsDone = true;
}

void TrainEngineer1Action::Cancel()
{
	bIsDone = true;
}

bool TrainEngineer1Action::IsDone()
{
	return bIsDone;
}