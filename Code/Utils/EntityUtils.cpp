#include "StdAfx.h"
#include "EntityUtils.h"
#include "GamePlugin.h"

#include <CryGame/IGameFramework.h>
#include <CryAISystem/INavigationSystem.h>
#include <CryAISystem/NavigationSystem/INavigationUpdatesManager.h>
#include <Components/BaseBuilding/Building.h>
#include <Components/Effects/BulletTracer.h>
#include <Components/ResourcePoints/BaseResourcePoint.h>

#include <Utils/MathUtils.h>

#include <Components/Info/OwnerInfo.h>
#include <CryEntitySystem/IEntitySystem.h>
#include <Components/Managers/ResourceManager.h>
#include <Components/Selectables/ResourceStorage.h>

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
	//CryLog("entity spawned and owner set...");

	return pSpawnedEntity;
}

void EntityUtils::RemoveEntity(IEntity* entity)
{
	OwnerInfoComponent* pOwnerInfo = entity->GetOrCreateComponent<OwnerInfoComponent>();
	if (!pOwnerInfo) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "EntityUtils :(RemoveEntity) No OwnerInfo Found on entity !");
		return;
	}
	//TODO : error pure function call vaghti owner remove shode (?)
	IEntity* pOwner = pOwnerInfo->GetOwner();
	if (!pOwner) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "EntityUtils :(RemoveEntity) owner Cannot be Empty !");
		return;
	}
	ResourceManagerComponent* pResourceManagerComponent = pOwner->GetComponent<ResourceManagerComponent>();
	if (!pResourceManagerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "EntityUtils :(SpawnEntity) No ResourceManagerComponent found on OwnerEntity");
		return;
	}

	pResourceManagerComponent->RemoveOwnedEntity(entity);
	gEnv->pEntitySystem->RemoveEntity(entity->GetId());

	if (entity->GetComponent<BuildingComponent>()) {
		AABB aabb;
		entity->GetWorldBounds(aabb);
		gEnv->pAISystem->GetNavigationSystem()->GetUpdateManager()->EntityChanged(entity->GetId(), aabb);
	}
}

f32 EntityUtils::GetDistance(Vec3 from, Vec3 to, IEntity* toEntity)
{
	if (toEntity) {
		AABB aabb;
		toEntity->GetWorldBounds(aabb);

		IPersistantDebug* pd = gEnv->pGameFramework->GetIPersistantDebug();
		pd->Begin("tessadadsadt", true);
		/*
		f32 distToCenterX = crymath::abs(aabb.GetCenter().x - from.x);
		f32 distToCenterY = crymath::abs(aabb.GetCenter().y - from.y);
		
		if (distToCenterX >= distToCenterY) {
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
		*/

		Vec3 pos = GetClosetPointOnMeshBorder(from, toEntity);
		pd->AddSphere(pos, 0.4f, ColorF(1, 0, 0), 0.4f);
		return from.GetDistance(pos);
	}
	else {
		return from.GetDistance(to);
	}
}

f32 EntityUtils::GetDistance(Vec3 from, Vec3 to)
{
	return from.GetDistance(to);
}

Vec3 EntityUtils::GetClosetPointOnMeshBorder(Vec3 from, IEntity* entity)
{
	if (entity) {
		AABB aabb;
		entity->GetWorldBounds(aabb);
		Vec3 to = entity->GetWorldPos();

		//f32 distToCenterX = crymath::abs(aabb.GetCenter().x - from.x);
	//	f32 distToCenterY = crymath::abs(aabb.GetCenter().y - from.y);

		IPersistantDebug* pd = gEnv->pGameFramework->GetIPersistantDebug();
		pd->Begin("tessadadsadt1231", true);
		/*
		if (distToCenterX >= distToCenterY) {
			f32 diff = aabb.max.x - aabb.GetCenter().x;
			Vec3 pos = to;
			if (from.x > to.x) {
				pos.x += diff;
			}
			else {
				pos.x -= diff;
			}
			pos.y = from.y;
			pd->AddSphere(pos, 0.4f, ColorF(0, 1, 0), 0.4f);
			return pos;
		}
		else {
			f32 diff = aabb.max.y - aabb.GetCenter().y;
			Vec3 pos = to;
			if (from.x > to.x) {
				pos.x += diff;
			}
			else {
				pos.x -= diff;
			}
			pos.x = from.x;
			pd->AddSphere(pos, 0.4f, ColorF(0, 1, 0), 0.4f);
			return pos;
		}
		*/

		f32 diffX = aabb.max.x - aabb.GetCenter().x;
		f32 diffY = aabb.max.y - aabb.GetCenter().y;

		Vec3 pos = to;
		//X
		if (from.x > pos.x + diffX)  {
			pos.x += diffX;
		}
		else if (from.x < pos.x - diffX) {
			pos.x -= diffX;
		}
		else {
			pos.x = from.x;
		}

		//Y
		if (from.y > pos.y + diffY) {
			pos.y += diffY;
		}
		else if (from.y < pos.y - diffY) {
			pos.y -= diffY;
		}
		else {
			pos.y = from.y;
		}
		//pd->AddSphere(pos, 0.4f, ColorF(0, 1, 0), 0.4f);
		return pos;
	}
	else {
		return ZERO;
	}
}

Vec3 EntityUtils::GetRandomPointOnMeshBorder(IEntity* entity)
{
	AABB aabb;
	entity->GetWorldBounds(aabb);

	f32 diffX = crymath::abs(aabb.max.x - aabb.min.x);
	f32 diffY = crymath::abs(aabb.max.y - aabb.min.y);


	Vec3 result = aabb.min;
	//result.x = (aabb.GetCenter().x + MathUtils::GetRandomFloat(-diffX, diffX));
	//result.y = (aabb.GetCenter().y + MathUtils::GetRandomFloat(-diffY, diffY));

	IPersistantDebug* pd = gEnv->pGameFramework->GetIPersistantDebug();
	pd->Begin("testset2342424", true);

	//f32 minX = aabb.min.x;
	//f32 maxX = aabb.max.x;
	//f32 minY = aabb.min.y;
	//f32 maxY = aabb.max.y;
	
	int32 n = MathUtils::GetRandomInt(1, 4);
	//X
	if (n == 1) {
		//f32 xPos = minX;
		result.y = aabb.min.y;
		result.x += MathUtils::GetRandomFloat(0, diffX);
	}
	else if (n == 2) {
		//f32 xPos = minX;
		result.y = aabb.max.y;
		result.x += MathUtils::GetRandomFloat(0, diffX);
	}
	else if (n == 3) {
		//f32 yPos = minY;
		result.x = aabb.min.x;
		result.y += MathUtils::GetRandomFloat(0, diffY);
	}
	else if (n == 4) {
		//f32 yPos = minY;
		result.x = aabb.max.x;
		result.y += MathUtils::GetRandomFloat(0, diffY);
	}

	//pd->AddSphere(result, 0.5f, ColorF(0, 0, 1), 5.5f);
	return result;
}

/*
Vec2 EntityUtils::GetScreenPosition(Cry::DefaultComponents::CCameraComponent* camera, IEntity* entity)
{
	Vec3 pos = entity->GetPos();
	//pos.z = 0;
	Vec3 result;
	camera->GetCamera().Project(pos, result);
	//gEnv->pRenderer->ProjectToScreen(pos.x, pos.y, pos.z, &result.x, &result.y, &result.z);
	//result.y = gEnv->pRenderer->GetHeight() - result.y;
	return Vec2(result.x, result.y);
}
*/
bool EntityUtils::IsEntityInsideViewPort(Cry::DefaultComponents::CCameraComponent* camera, IEntity* entity)
{
	Vec3 pos = entity->GetWorldPos();
	Vec3 result;
	camera->GetCamera().Project(pos, result);

	int32 vHeight = gEnv->pRenderer->GetHeight();
	int32 vWidth = gEnv->pRenderer->GetWidth();

	return result.x >= 0 && result.x <= vWidth && result.y >= 0 && result.y <= vHeight;
}

IEntity* EntityUtils::FindClosestWarehouse(IEntity* to)
{
	IEntityItPtr entityItPtr = gEnv->pEntitySystem->GetEntityIterator();
	entityItPtr->MoveFirst();
	while (!entityItPtr->IsEnd()) {
		IEntity* entity = entityItPtr->Next();
		if (entity) {
			/*
			BuildingComponent* pBuildingComponent = entity->GetComponent<BuildingComponent>();
			if (!pBuildingComponent) {
				return nullptr;
			}
			if (!pBuildingComponent->IsBuilt()) {
				return nullptr;
			}
			*/
			ResourceStorageComponent* resourceStorage = entity->GetComponent<ResourceStorageComponent>();
			if (resourceStorage) {
				return entity;
			}
		}
	}
	return nullptr;
}

IEntity* EntityUtils::GetClosestEntity(DynArray<IEntity*> entities, Vec3 to)
{
	f32 closest = 10000;
	IEntity* result = nullptr;
	for (IEntity* entity : entities) {
		f32 distance = to.GetDistance(entity->GetWorldPos());
		if (distance < closest) {
			closest = distance;
			result = entity;
		}
	}
	return result;
}

DynArray<IEntity*> EntityUtils::FindHostilePlayers(IEntity* to)
{
	DynArray<IEntity*> m_hostilePlayers;
	IEntityItPtr entityItPtr = gEnv->pEntitySystem->GetEntityIterator();
	entityItPtr->MoveFirst();

	while (!entityItPtr->IsEnd())
	{
		IEntity* entity = entityItPtr->Next();
		ResourceManagerComponent* pResourceManagerComponent = entity->GetComponent<ResourceManagerComponent>();
		if (!pResourceManagerComponent) {
			continue;
		}

		OwnerInfoComponent* otherEntityOwnerInfo = entity->GetComponent<OwnerInfoComponent>();
		BulletTracerComponent* bulletTracerComponent = entity->GetComponent<BulletTracerComponent>();
		//Ignore entity if it's not in detection range
		if (!otherEntityOwnerInfo || bulletTracerComponent) {
			continue;
		}

		//set entity as randomAttackTarget if it's team is not same as this unit's team
		OwnerInfoComponent* pOwnerInfoComponent = to->GetComponent<OwnerInfoComponent>();
		if (pOwnerInfoComponent && otherEntityOwnerInfo && otherEntityOwnerInfo->GetInfo().m_pTeam != pOwnerInfoComponent->GetInfo().m_pTeam) {
			m_hostilePlayers.append(entity);
		}
	}
	return m_hostilePlayers;
}

IEntity* EntityUtils::FindAvailableResourcePointOfType(EResourceType type, IEntity* collector)
{
	IEntityItPtr entityItPtr = gEnv->pEntitySystem->GetEntityIterator();
	entityItPtr->MoveFirst();

	DynArray<IEntity*> points;
	while (!entityItPtr->IsEnd()) {
		IEntity* entity = entityItPtr->Next();
		if (entity) {
			BaseResourcePointComponent* pResourcePointComponent = entity->GetComponent<BaseResourcePointComponent>();
			if (pResourcePointComponent && pResourcePointComponent->GetType() == type && !pResourcePointComponent->IsCollectorsSlotsFull()) {
				points.append(entity);
			}
		}
	}
	return GetClosestEntity(points, collector->GetWorldPos());
}

