#pragma once

#include <Actions/IBaseAction.h>

class AIControllerComponent;
class UnitStateManagerComponent;
class WorkplaceComponent;
class WorkerComponent;

class UnitWorkInWorkplaceAction : public IBaseAction {

public:
	UnitWorkInWorkplaceAction(IEntity* entity, IEntity* workplace);

private:
	AIControllerComponent* m_pAiControllerComponent = nullptr;
	UnitStateManagerComponent* m_pStateManagerComponent = nullptr;
	WorkplaceComponent* m_pWorkplaceComponent = nullptr;
	WorkerComponent* m_pWorkerComponent = nullptr;

	IEntity* m_pWorkplaceEntity = nullptr;
	int32 m_workerIndex = -1;

private:
	bool isAssignedToWorkplace = false;

private:

public:
	virtual void Execute();
	virtual void Cancel();
	virtual bool IsDone();
	virtual bool CanBeSkipped();
};
