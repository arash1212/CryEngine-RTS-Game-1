#pragma once

#include <Actions/IBaseAction.h>

class AIControllerComponent;
class UnitStateManagerComponent;
class GuardPostComponent;
class GuardComponent;

class UnitGuardInGuardPostAction : public IBaseAction {

public:
	UnitGuardInGuardPostAction(IEntity* entity, IEntity* workplace);

private:
	AIControllerComponent* m_pAiControllerComponent = nullptr;
	UnitStateManagerComponent* m_pStateManagerComponent = nullptr;
	GuardPostComponent* m_pGuardPostComponent = nullptr;
	GuardComponent* m_pGuardComponent = nullptr;

	IEntity* m_pGuardPostEntity = nullptr;
	int32 nGuardIndex = -1;

private:
	bool isAssignedToWorkplace = false;

private:

public:
	virtual void Execute() override;
	virtual void Cancel() override;
	virtual bool IsDone() override;
	virtual bool CanBeSkipped() override;
};

