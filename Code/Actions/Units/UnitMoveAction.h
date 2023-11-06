#pragma once

#include <Actions/IBaseAction.h>

class AIControllerComponent;
class AttackerComponent;
class UnitStateManagerComponent;

class UnitMoveAction : public IBaseAction {

public:
	UnitMoveAction(IEntity* entity, Vec3 movePosition, bool run);

private:
	AIControllerComponent* m_pAiControllerComponent = nullptr;
	AttackerComponent* m_pAttackerComponent = nullptr;
	UnitStateManagerComponent* m_pStateManagerComponent = nullptr;

private:
	Vec3 m_movePosition = ZERO;
	bool bRun = false;

public:
	virtual void Execute() override;
	virtual void Cancel() override;
	virtual bool IsDone() override;
};