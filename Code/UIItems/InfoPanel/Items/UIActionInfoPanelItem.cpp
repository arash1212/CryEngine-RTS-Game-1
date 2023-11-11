#include "StdAfx.h"
#include "UIActionInfoPanelItem.h"
#include "GamePlugin.h"

#include <Components/Player/Player.h>
#include <Components/Managers/ActionManager.h>

#include <CryEntitySystem/IEntitySystem.h>
#include <Components/BaseBuilding/Building.h>
#include <Components/Selectables/Cost.h>
#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/ResourceManager.h>
#include <Components/Selectables/Units/Engineer1Unit.h>
#include <Components/Managers/UnitTypeManager.h>

#include <Actions/Train/TrainEngineer1Action.h>
#include <Components/Managers/ActionManager.h>

UIActionInfoPanelItem::UIActionInfoPanelItem(IEntity* entity, string imagePath, IBaseAction* action)
{
	this->m_pEntity = entity;
	this->m_imagePath = imagePath;
	this->m_pAction = action;
}

void UIActionInfoPanelItem::Execute()
{
	if (!m_pAction) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIBuildingActionInfoPanelItem : (Execute) m_pAction is null !");
		return;
	}
	ActionManagerComponent* pActionManagerComponent = m_pEntity->GetComponent< ActionManagerComponent>();
	if (!pActionManagerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIBuildingActionInfoPanelItem : (Execute) pActionManagerComponent is null !");
		return;
	}
	m_pAction->Cancel();
}

string UIActionInfoPanelItem::GetImagePath()
{
	return m_imagePath;
}