#pragma once

#include <Actions/IBaseAction.h>

class MoveAction : public IBaseAction {

public:
	MoveAction(IEntity* entity, Vec3 movePosition);

private:
	Vec3 m_movePosition = ZERO;

public:
	virtual void Execute();
	virtual void Cancel();
	virtual bool IsDone();
};