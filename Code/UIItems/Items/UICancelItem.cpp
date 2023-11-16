#include "StdAfx.h"
#include "UICancelItem.h"
#include "GamePlugin.h"

#include <Components/Managers/ActionManager.h>

UICancelItem::UICancelItem(IEntity* entity)
{
	this->m_pEntity = entity;
}

void UICancelItem::Execute()
{
	if (!m_pEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UICancelAction : (Execute) m_pEntity is null !");
		return;
	}
	ActionManagerComponent* actionManager = m_pEntity->GetComponent<ActionManagerComponent>();
	if (actionManager) {
		actionManager->CancelCurrentAction();
	}
}

string UICancelItem::GetImagePath()
{
	return this->GetDescrption().sIcon;
}

SDescription UICancelItem::GetDescrption()
{
	SResourceInfo cost;

	SDescription description;
	description.sDescription = "Cancel Selected Units Action";
	description.sBuyDescription = "Cancel Selected Units Action";
	description.sIcon = "cancel.png";
	return description;
}
