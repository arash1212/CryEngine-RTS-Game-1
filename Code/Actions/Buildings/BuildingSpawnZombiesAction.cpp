#include "StdAfx.h"
#include "BuildingSpawnZombiesAction.h"
#include "GamePlugin.h"

#include <CryEntitySystem/IEntitySystem.h>
#include <Utils/EntityUtils.h>
#include <Components/Managers/ResourceManager.h>
#include <Components/BaseBuilding/Building.h>

#include <Actions/Units/UnitWanderingRandomlyAction.h>
#include <Actions/Units/UnitAttackEnemyBaseAction.h>
#include <Actions/Buildings/BuildingSpawnZombiesAction.h>
#include <Components/Managers/ActionManager.h>

#include <Components/Selectables/Attacker.h>
#include <Components/Controller/AIController.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Selectables/Units/Zombie1Unit.h>
#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/ResourceManager.h>
#include <Components/Selectables/Cost.h>

BuildingSpawnZombiesAction::BuildingSpawnZombiesAction(IEntity* entity, int32 maxZombiesCount)
{
	this->m_pEntity = entity;
	//this->m_exitPosition = exitPos;
	this->m_maxZombiesCount = maxZombiesCount;
}

void BuildingSpawnZombiesAction::Execute()
{
	if (!m_pEntity || m_pEntity->IsGarbage()) {
		Cancel();
		return;
	}
	OwnerInfoComponent* pOnwerInfo = m_pEntity->GetComponent<OwnerInfoComponent>();
	if (!pOnwerInfo) {
		return;
	}
	IEntity* ownerEntity = pOnwerInfo->GetOwner();
	if (!ownerEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "BuildingSpawnZombiesAction:(Execute) ownerEntity is null.");
		Cancel();
		return;
	}
	ResourceManagerComponent* pResourceManagerComponent = m_pEntity->GetOrCreateComponent<ResourceManagerComponent>();
	if (!pResourceManagerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "BuildingSpawnZombiesAction:(Execute) ResourceManagerComponent is null.");
		Cancel();
		return;
	}
	BuildingComponent* pBuildingComponent = m_pEntity->GetOrCreateComponent<BuildingComponent>();
	if (!pBuildingComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "BuildingSpawnZombiesAction:(Execute) BuildingComponent is null.");
		Cancel();
		return;
	}
	if (pResourceManagerComponent->GetOwnedEntities().size() >= m_maxZombiesCount) {
		return;
	}

	if (m_spawnTimePassed < m_timeBetweenSpawningZombies) {
		m_spawnTimePassed += 0.5f * gEnv->pTimer->GetFrameTime();
		return;
	}

	if (m_spawnTimePassed >= m_timeBetweenSpawningZombies) {
		Vec3 position = m_pEntity->GetWorldPos();
		Quat rotation = IDENTITY;

		IEntity* spawnedEntity = EntityUtils::SpawnEntity(pBuildingComponent->GetExitPoint(), rotation, m_pEntity);
		if (!spawnedEntity) {
			return;
		}
		spawnedEntity->GetOrCreateComponent<Zombie1UnitComponent>();

		ActionManagerComponent* pActionManagerComponent = spawnedEntity->GetComponent<ActionManagerComponent>();
		pActionManagerComponent->AddAction(new UnitWanderingRandomlyAction(spawnedEntity, m_pEntity->GetWorldPos(), false));
		m_spawnTimePassed = 0;
	}
}

void BuildingSpawnZombiesAction::Cancel()
{
	bIsDone = true;
}

bool BuildingSpawnZombiesAction::IsDone()
{
	return bIsDone;
}

f32 BuildingSpawnZombiesAction::GetProgressAmount()
{
	return crymath::floor(m_spawnTimePassed);
}


f32 BuildingSpawnZombiesAction::GetMaxProgressAmount()
{
	return crymath::floor(m_timeBetweenSpawningZombies);
}


