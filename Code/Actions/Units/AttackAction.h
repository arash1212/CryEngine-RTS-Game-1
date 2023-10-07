#pragma once

#include <Actions/IBaseAction.h>

class AttackAction : public IBaseAction {

public:
	AttackAction(IEntity* entity, IEntity* target);

private:
	IEntity* m_pTarget = nullptr;

public:
	virtual void Execute();
	virtual void Cancel();
	virtual bool IsDone();
};