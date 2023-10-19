#pragma once

struct IEntity;

class EntityUtils {
public:
	static IEntity* SpawnEntity(Vec3 position, Quat rotation, IEntity* owner);
};