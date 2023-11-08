#include "StdAfx.h"
#include "UIUnitInfoPanelItem.h"
#include "GamePlugin.h"

#include <Components/Player/Player.h>

#include <CryEntitySystem/IEntitySystem.h>
#include <Components/BaseBuilding/Building.h>
#include <Components/Selectables/Cost.h>
#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/ResourceManager.h>
#include <Components/Selectables/Units/Engineer1Unit.h>
#include <Components/Managers/UnitTypeManager.h>

#include <Actions/Train/TrainEngineer1Action.h>
#include <Components/Managers/ActionManager.h>

UIUnitInfoPanelItem::UIUnitInfoPanelItem(IEntity* entity, string imagePath)
{
	this->m_pEntity = entity;
	this->m_imagePath = imagePath;
}

void UIUnitInfoPanelItem::Execute()
{
	//TODO
	IEntity* player = gEnv->pEntitySystem->FindEntityByName(PLAYER_ENTITY_NAME);
	if (!player) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIUnitInfoPanelItem : (Execute) Player is null !");
		return;
	}
	PlayerComponent* pPlayerComponent = player->GetComponent<PlayerComponent>();
	if (!pPlayerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIUnitInfoPanelItem : (Execute) pPlayerComponent is null !");
		return;
	}
	UnitTypeManagerComponent* pUnitTypeManagerComponent = m_pEntity->GetComponent< UnitTypeManagerComponent>();
	if (!pUnitTypeManagerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIUnitInfoPanelItem : (Execute) pUnitTypeManagerComponent is null !");
		return;
	}

	pPlayerComponent->DeselectUnitsOfType(pUnitTypeManagerComponent->GetUnitType());
}

string UIUnitInfoPanelItem::GetImagePath()
{
	return m_imagePath;
}