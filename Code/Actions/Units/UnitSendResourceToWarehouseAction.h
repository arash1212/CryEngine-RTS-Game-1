#pragma once

#include <Actions/IBaseAction.h>

class AIControllerComponent;
class EngineerComponent;
class UnitStateManagerComponent;
class ResourceComponent;
class UnitAnimationComponent;
class ResourceCollectorComponent;

class UnitSendResourceToWarehouseAction : public IBaseAction {

public:
	UnitSendResourceToWarehouseAction(IEntity* entity, IEntity* warehouseEntity);

private:
	AIControllerComponent* m_pAiControllerComponent = nullptr;
	UnitStateManagerComponent* m_pStateManagerComponent = nullptr;
	UnitAnimationComponent* m_pAnimationComponent = nullptr;
	ResourceCollectorComponent* m_pResourceCollectorComponent = nullptr;

	IEntity* m_pWarehouseEntity = nullptr;
private:
	Vec3 m_movePosition = ZERO;
	bool bRun = false;

public:
	virtual void Execute();
	virtual void Cancel();
	virtual bool IsDone();
};