#pragma once

#include <Actions/IBaseAction.h>

class AIControllerComponent;
class AttackerComponent;
class UnitStateManagerComponent;
class IBaseInfoPanelUIItem;

class TrainEngineer1Action : public IBaseAction {

public:
	TrainEngineer1Action(IEntity* entity, Vec3 exitPos);

private:
	IEntity* m_pTarget = nullptr;
	Vec3 m_exitPosition = ZERO;

	f32 m_processAmount = 0.f;
	//Timer
	f32 m_timeBetweenProcess = 0.5f;
	f32 m_ProcessTimePassesd = 0.0f;

public:
	virtual void Execute() override;
	virtual void Cancel() override;
	virtual bool IsDone() override;
	virtual f32 GetProgressAmount() override;
	virtual IBaseInfoPanelUIItem* GetInfoPanelItem() override;
};