#pragma once

#include <Actions/IBaseAction.h>

class AIControllerComponent;
class AttackerComponent;
class UnitStateManagerComponent;
class ResourceManagerComponent;

class UnitAttackEnemyBaseAction : public IBaseAction {

public:
	UnitAttackEnemyBaseAction(IEntity* entity, IEntity* target);

private:
	IEntity* m_pTarget = nullptr;
	AIControllerComponent* m_pAiControllerComponent = nullptr;
	AttackerComponent* m_pAttackerComponent = nullptr;
	ResourceManagerComponent* m_pTargetResourceManagerComponent = nullptr;

private:

public:
	virtual void Execute() override;
	virtual void Cancel() override;
	virtual bool IsDone() override;
	virtual bool CanBeSkipped() override;
};
