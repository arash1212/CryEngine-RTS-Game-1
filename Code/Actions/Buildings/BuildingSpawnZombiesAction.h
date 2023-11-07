#pragma once

#include <Actions/IBaseAction.h>

class AIControllerComponent;
class AttackerComponent;
class UnitStateManagerComponent;

class BuildingSpawnZombiesAction : public IBaseAction {

public:
	BuildingSpawnZombiesAction(IEntity* entity, int32 maxZombiesCount);

private:
	IEntity* m_pTarget = nullptr;
	Vec3 m_exitPosition = ZERO;

	f32 m_timeBetweenSpawningZombies = 5.f;
	f32 m_spawnTimePassed = -0.f;
	int32 m_maxZombiesCount = 40;

	f32 m_processAmount = 0.f;

public:
	virtual void Execute() override;
	virtual void Cancel() override;
	virtual bool IsDone() override;
	virtual f32 GetProgressAmount() override;
	virtual f32 GetMaxProgressAmount() override;
};
