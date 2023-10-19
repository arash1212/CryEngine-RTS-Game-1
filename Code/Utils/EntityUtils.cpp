#include "StdAfx.h"
#include "EntityUtils.h"
#include "GamePlugin.h"

#include <Components/Info/OwnerInfo.h>
#include <CryEntitySystem/IEntitySystem.h>
#include <Components/Managers/ResourceManager.h>

IEntity* EntityUtils::SpawnEntity(Vec3 position, Quat rotation, IEntity* owner)
{
	SEntitySpawnParams entitySpawnParams;
	entitySpawnParams.vPosition = position;
	entitySpawnParams.qRotation = rotation;
	IEntity* pSpawnedEntity = gEnv->pEntitySystem->SpawnEntity(entitySpawnParams, true);

	if (!pSpawnedEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "EntityUtils :(SpawnEntity) No entities spawned !");
		return nullptr;
	}
	if (!owner) {
		return pSpawnedEntity;
	}

	ResourceManagerComponent* pResourceManagerComponent = owner->GetComponent<ResourceManagerComponent>();
	if (!pResourceManagerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "EntityUtils :(SpawnEntity) No ResourceManagerComponent found on OwnerEntity");
		return nullptr;
	}

	OwnerInfoComponent* pOwnerInfo = pSpawnedEntity->GetOrCreateComponent<OwnerInfoComponent>();
	pOwnerInfo->SetOwner(owner);
	pResourceManagerComponent->AddOwnedEntity(pSpawnedEntity);
	CryLog("entity spawned and owner set...");

	return pSpawnedEntity;
}
