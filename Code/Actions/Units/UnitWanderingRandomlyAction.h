#pragma once

#include <Actions/IBaseAction.h>

class AIControllerComponent;
class AttackerComponent;
class UnitStateManagerComponent;

class UnitWanderingRandomlyAction : public IBaseAction {

public:
	UnitWanderingRandomlyAction(IEntity* entity, IEntity* around, bool run);

private:
	AIControllerComponent* m_pAiControllerComponent = nullptr;
	AttackerComponent* m_pAttackerComponent = nullptr;
	UnitStateManagerComponent* m_pStateManagerComponent = nullptr;
	IEntity* m_pAround = nullptr;

private:
	Vec3 m_movePosition = ZERO;
	bool bRun = false;

public:
	virtual void Execute();
	virtual void Cancel();
	virtual bool IsDone();
};