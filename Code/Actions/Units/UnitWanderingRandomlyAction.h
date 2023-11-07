#pragma once

#include <Actions/IBaseAction.h>

class AIControllerComponent;
class AttackerComponent;
class UnitStateManagerComponent;

class UnitWanderingRandomlyAction : public IBaseAction {

public:
	UnitWanderingRandomlyAction(IEntity* entity, Vec3 around, bool run);

private:
	AIControllerComponent* m_pAiControllerComponent = nullptr;
	AttackerComponent* m_pAttackerComponent = nullptr;
	UnitStateManagerComponent* m_pStateManagerComponent = nullptr;
	Vec3 m_pAround = ZERO;

private:
	Vec3 m_movePosition = ZERO;
	bool bRun = false;

public:
	virtual void Execute() override;
	virtual void Cancel() override;
	virtual bool IsDone() override;
	virtual bool CanBeSkipped() override;
};