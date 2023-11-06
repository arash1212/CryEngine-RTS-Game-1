#pragma once

#include <Actions/IBaseAction.h>

class AIControllerComponent;
class AttackerComponent;
class UnitStateManagerComponent;

class UnitAttackAction : public IBaseAction {

public:
	UnitAttackAction(IEntity* entity, IEntity* target);

private:
	IEntity* m_pTarget = nullptr;
	AIControllerComponent* m_pAiControllerComponent = nullptr;
	AttackerComponent* m_pAttackerComponent = nullptr;
	UnitStateManagerComponent* m_pStateManagerComponent = nullptr;


public:
	virtual void Execute() override;
	virtual void Cancel() override;
	virtual bool IsDone() override;
};