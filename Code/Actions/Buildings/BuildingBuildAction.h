#pragma once

#include <Actions/IBaseAction.h>

class AIControllerComponent;
class AttackerComponent;
class UnitStateManagerComponent;
class BuildingComponent;

class BuildingBuildAction : public IBaseAction {

public:
	BuildingBuildAction(IEntity* entity);

private:
	BuildingComponent* m_pBuildingComponent = nullptr;

public:
	virtual void Execute() override;
	virtual void Cancel() override;
	virtual bool IsDone() override;
	virtual f32 GetProgressAmount() override;
	virtual f32 GetMaxProgressAmount() override;
};

