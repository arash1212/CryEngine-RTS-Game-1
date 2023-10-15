#include "StdAfx.h"
#include "UITrainEngineer1Item.h"
#include "GamePlugin.h"

#include <CryEntitySystem/IEntitySystem.h>
#include <Components/BaseBuilding/Building.h>

#include <Actions/Train/TrainEngineer1Action.h>
#include <Components/Managers/ActionManager.h>

UITrainEngineer1Item::UITrainEngineer1Item(IEntity* entity)
{
	this->m_pEntity = entity;
}

void UITrainEngineer1Item::Execute()
{
	if (!m_pEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UITrainEngineer1Item : (Execute) m_pEntity is null !");
		return;
	}
	BuildingComponent* building = m_pEntity->GetComponent<BuildingComponent>();
	if (!building) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UITrainEngineer1Item : (Execute) buildingComponent is null !");
		return;
	}
	ActionManagerComponent* actionManager = m_pEntity->GetComponent<ActionManagerComponent>();
	if (!actionManager) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UITrainEngineer1Item : (Execute) actionManager is null !");
		return;
	}

	if (actionManager) {
		actionManager->AddAction(new TrainEngineer1Action(m_pEntity, building->GetExitPoint()));
	}
}

string UITrainEngineer1Item::GetImagePath()
{
	return "engineer_1_icon.png";
}