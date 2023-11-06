#pragma once

#include <Actions/IBaseAction.h>

class AIControllerComponent;
class EngineerComponent;
class UnitStateManagerComponent;
class ResourceComponent;
class UnitAnimationComponent;
class ResourceCollectorComponent;

class UnitCollectResourceAction : public IBaseAction {

public:
	UnitCollectResourceAction(IEntity* entity, IEntity* resourceEntity);

private:
	AIControllerComponent* m_pAiControllerComponent = nullptr;
	EngineerComponent* m_pEngineerComponent = nullptr;
	UnitStateManagerComponent* m_pStateManagerComponent = nullptr;
	ResourceComponent* m_pResourceComponent = nullptr;
	UnitAnimationComponent* m_pAnimationComponent = nullptr;
	ResourceCollectorComponent* m_pResourceCollectorComponent = nullptr;

	IEntity* m_pResourceEntity = nullptr;
	IEntity* m_pWarehouseEntity = nullptr;
private:
	Vec3 m_movePosition = ZERO;
	bool bRun = false;

	//Timers
	f32 m_builtTimePassed = 0.f;
public:
	virtual void Execute() override;
	virtual void Cancel() override;
	virtual bool IsDone() override;
};
