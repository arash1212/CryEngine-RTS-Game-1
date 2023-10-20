#include "StdAfx.h"
#include "EntityUtils.h"
#include "GamePlugin.h"

#include <CryGame/IGameFramework.h>

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

void EntityUtils::RemoveEntity(IEntity* entity)
{
	OwnerInfoComponent* pOwnerInfo = entity->GetOrCreateComponent<OwnerInfoComponent>();
	if (!pOwnerInfo) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "EntityUtils :(RemoveEntity) No OwnerInfo Found on entity !");
		return;
	}
	IEntity* owner = pOwnerInfo->GetOwner();
	if (!owner) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "EntityUtils :(RemoveEntity) owner Cannot be Empty !");
		return;
	}
	ResourceManagerComponent* pResourceManagerComponent = pOwnerInfo->GetOwner()->GetComponent<ResourceManagerComponent>();
	if (!pResourceManagerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "EntityUtils :(SpawnEntity) No ResourceManagerComponent found on OwnerEntity");
		return;
	}

	pResourceManagerComponent->RemoveOwnedEntity(entity);
	gEnv->pEntitySystem->RemoveEntity(entity->GetId());
}

f32 EntityUtils::GetDistance(Vec3 from, Vec3 to, IEntity* toEntity)
{
	if (toEntity) {
		AABB aabb;
		toEntity->GetWorldBounds(aabb);

		f32 distToCenterX = crymath::abs(aabb.GetCenter().x - from.x);
		f32 distToCenterY = crymath::abs(aabb.GetCenter().y - from.y);

		IPersistantDebug* pd = gEnv->pGameFramework->GetIPersistantDebug();
		pd->Begin("tessadadsadt", true);
		if (distToCenterX > distToCenterY) {
			f32 diff = aabb.max.x - aabb.GetCenter().x;
			Vec3 pos = to;
			if (from.x > to.x) {
				pos.x += diff;
			}
			else {
				pos.x -= diff;
			}
			pos.y = from.y;
			pd->AddSphere(pos, 0.4f, ColorF(1, 0, 0), 0.4f);
			return from.GetDistance(pos);
		}
		else {
			f32 diff = aabb.max.y - aabb.GetCenter().y;
			Vec3 pos = to;
			if (from.y > to.y) {
				pos.y += diff;
			}
			else {
				pos.y -= diff;
			}
			pos.x = from.x;
			pd->AddSphere(pos, 0.4f, ColorF(1, 0, 0), 0.4f);
			return from.GetDistance(pos);
		}
	}
	else {
		return from.GetDistance(to);
	}
}

Vec3 EntityUtils::GetClosetPointOnMeshBorder(Vec3 from, IEntity* entity)
{
	if (entity) {
		AABB aabb;
		entity->GetWorldBounds(aabb);
		Vec3 to = entity->GetWorldPos();

		f32 distToCenterX = crymath::abs(aabb.GetCenter().x - from.x);
		f32 distToCenterY = crymath::abs(aabb.GetCenter().y - from.y);

		IPersistantDebug* pd = gEnv->pGameFramework->GetIPersistantDebug();
		pd->Begin("tessadadsadt1231", true);
		if (distToCenterX > distToCenterY) {
			f32 diff = aabb.max.x - aabb.GetCenter().x;
			Vec3 pos = to;
			if (from.x > to.x) {
				pos.x += diff;
			}
			else {
				pos.x -= diff;
			}
			pos.y = from.y;
			pd->AddSphere(pos, 0.4f, ColorF(1, 0, 0), 0.4f);
			return pos;
		}
		else {
			f32 diff = aabb.max.y - aabb.GetCenter().y;
			Vec3 pos = to;
			if (from.y > to.y) {
				pos.y += diff;
			}
			else {
				pos.y -= diff;
			}
			pos.x = from.x;
			pd->AddSphere(pos, 0.4f, ColorF(1, 0, 0), 0.4f);
			return pos;
		}
	}
	else {
		return ZERO;
	}
}
