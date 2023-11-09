#include "StdAfx.h"
#include "UIChangeStanceItem.h"
#include "GamePlugin.h"

#include <Components/Managers/UnitStateManager.h>

UIChangeStanceItem::UIChangeStanceItem(IEntity* entity)
{
	this->m_pEntity = entity;
}

void UIChangeStanceItem::Execute()
{
	if (!m_pEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UICancelAction : (Execute) m_pEntity is null !");
		return;
	}
	UnitStateManagerComponent* stateManager = m_pEntity->GetComponent<UnitStateManagerComponent>();
	if (stateManager) {
		EUnitStance stance = stateManager->GetStance();
		stateManager->SetStance(static_cast<EUnitStance>((static_cast<int>(stance) + 1) % (static_cast<int>(EUnitStance::PRONE) + 1)));
	}
}

string UIChangeStanceItem::GetImagePath()
{
	return "UI_Stance_Icon.png";
}

SUIItemDescription UIChangeStanceItem::GetDescrption()
{
	SResourceInfo pInfo;
	return SUIItemDescription(pInfo, "Change Unit Stance.");
}
