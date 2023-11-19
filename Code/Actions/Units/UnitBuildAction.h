#pragma once

#include <Actions/IBaseAction.h>

class AIControllerComponent;
class EngineerComponent;
class UnitStateManagerComponent;
class BuildingComponent;
class UnitAnimationComponent;

class UnitBuildAction : public IBaseAction {

public:
	UnitBuildAction(IEntity* entity, IEntity* buildiingEntity);

private:
	AIControllerComponent* m_pAiControllerComponent = nullptr;
	EngineerComponent* m_pEngineerComponent = nullptr;
	UnitStateManagerComponent* m_pStateManagerComponent = nullptr;
	BuildingComponent* m_pBuildingComponent = nullptr;
	UnitAnimationComponent* m_pAnimationComponent = nullptr;

	IEntity* m_pBuildingEntity = nullptr;

private:
	Vec3 m_movePosition = ZERO;
	bool bRun = false;

	//Timers
	f32 m_builtTimePassed = 0.f;

private:
	bool IsMoveToPointAvailable();
	Vec3 GetClosestPointAvailableCloseToBuilding();
public:
	virtual void Execute() override;
	virtual void Cancel() override;
	virtual bool IsDone() override;
	virtual f32 GetProgressAmount() override;
	virtual f32 GetMaxProgressAmount() override;
};