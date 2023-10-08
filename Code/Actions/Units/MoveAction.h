#pragma once

#include <Actions/IBaseAction.h>

class MoveAction : public IBaseAction {

public:
	MoveAction(IEntity* entity, Vec3 movePosition, bool run);

private:
	Vec3 m_movePosition = ZERO;
	bool bRun = false;

public:
	virtual void Execute();
	virtual void Cancel();
	virtual bool IsDone();
};