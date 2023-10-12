#pragma once

#include <Actions/IBaseAction.h>

class AIControllerComponent;
class AttackerComponent;
class UnitStateManagerComponent;

class AttackAction : public IBaseAction {

public:
	AttackAction(IEntity* entity, IEntity* target);

private:
	IEntity* m_pTarget = nullptr;
	AIControllerComponent* m_pAiControllerComponent = nullptr;
	AttackerComponent* m_pAttackerComponent = nullptr;
	UnitStateManagerComponent* m_pStateManagerComponent = nullptr;


public:
	virtual void Execute();
	virtual void Cancel();
	virtual bool IsDone();
};