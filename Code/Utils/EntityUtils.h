#pragma once

#include <DefaultComponents/Cameras/CameraComponent.h>
struct IEntity;

class EntityUtils {
public:
	static IEntity* SpawnEntity(Vec3 position, Quat rotation, IEntity* owner);
	static void RemoveEntity(IEntity* entity);
	static f32 GetDistance(Vec3 from, Vec3 to, IEntity* toEntity);
	static Vec3 GetClosetPointOnMeshBorder(Vec3 from, IEntity* entity);
	static Vec3 GetRandomPointOnMeshBorder(IEntity* entity);
	//static Vec2 GetScreenPosition(Cry::DefaultComponents::CCameraComponent* camera, IEntity* entity);
	static bool IsEntityInsideViewPort(Cry::DefaultComponents::CCameraComponent* camera, IEntity* entity);
	static IEntity* FindClosestWarehouse(IEntity* to);
	static IEntity* GetClosestEntity(DynArray<IEntity*> entities, Vec3 to);
	static DynArray<IEntity*> FindHostilePlayers(IEntity* to);
};