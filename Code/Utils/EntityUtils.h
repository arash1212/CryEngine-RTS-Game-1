#pragma once

struct IEntity;

class EntityUtils {
public:
	static IEntity* SpawnEntity(Vec3 position, Quat rotation, IEntity* owner);
	static void RemoveEntity(IEntity* entity);
	static f32 GetDistance(Vec3 from, Vec3 to, IEntity* toEntity);
	static Vec3 GetClosetPointOnMeshBorder(Vec3 from, IEntity* entity);
};