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
		CryLog("UICancelAction : (Execute) m_pEntity is null !");
		return;
	}
	
	//If entity is a Unit
	ActionManagerComponent* actionManager = m_pEntity->GetComponent<ActionManagerComponent>();
	if (actionManager) {
		actionManager->CancelCurrentAction();
	}
}

string UICancelItem::GetImagePath()
{
	return "cancel.png";
}
