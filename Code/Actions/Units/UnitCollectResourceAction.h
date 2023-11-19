#pragma once

#include <Actions/IBaseAction.h>

class AIControllerComponent;
class EngineerComponent;
class UnitStateManagerComponent;
class BaseResourcePointComponent;
class UnitAnimationComponent;
class ResourceCollectorComponent;

class UnitCollectResourceAction : public IBaseAction {

public:
	UnitCollectResourceAction(IEntity* entity, IEntity* resourceEntity);

private:
	AIControllerComponent* m_pAiControllerComponent = nullptr;
	EngineerComponent* m_pEngineerComponent = nullptr;
	UnitStateManagerComponent* m_pStateManagerComponent = nullptr;
	BaseResourcePointComponent* m_pResourcePointComponent = nullptr;
	UnitAnimationComponent* m_pAnimationComponent = nullptr;
	ResourceCollectorComponent* m_pResourceCollectorComponent = nullptr;

	IEntity* m_pResourcePointEntity = nullptr;
	IEntity* m_pWarehouseEntity = nullptr;
private:
	Vec3 m_movePosition = ZERO;
	bool bRun = false;
	bool bResourcesAddedToCollector = false;
	int32 nCollectedAmount = 0;

	//Timers
	f32 fCollectingTimePassed = 0.f;

private:
	bool IsMoveToPointAvailable();
	Vec3 GetClosestPointAvailableCloseToBuilding();

public:
	virtual void Execute() override;
	virtual void Cancel() override;
	virtual bool IsDone() override;
};
