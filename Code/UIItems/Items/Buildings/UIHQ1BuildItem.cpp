#include "StdAfx.h"
#include "UIHQ1BuildItem.h"
#include "GamePlugin.h"

#include <Components/Player/Player.h>
#include <CryEntitySystem/IEntitySystem.h>
#include <Components/BaseBuilding/BaseBuilding.h>
#include <Components/BaseBuilding/Buildings/HQ1Building.h>

#include <Components/Managers/ActionManager.h>

UIHQ1BuildItem::UIHQ1BuildItem(IEntity* entity)
{
	this->m_pEntity = entity;
}

void UIHQ1BuildItem::Execute()
{
	if (!m_pEntity) {
		CryLog("UICancelAction : (Execute) m_pEntity is null !");
		return;
	}

	IEntity* m_pPlayerEntity = gEnv->pEntitySystem->FindEntityByName(PLAYER_ENTITY_NAME);
	if (!m_pPlayerEntity) {
		CryLog("UICancelAction : (Execute) m_pPlayerEntity is null !");
		return;
	}

	ActionManagerComponent* actionManager = m_pEntity->GetComponent<ActionManagerComponent>();
	if (actionManager) {
		IEntity* pBuildingEntity = m_pPlayerEntity->GetComponent<BaseBuildingComponent>()->AssignBuilding();
		if (pBuildingEntity) {
			pBuildingEntity->GetOrCreateComponent<HQ1BuildingComponent>();
		}
	}
}

string UIHQ1BuildItem::GetImagePath()
{
	return "hq_1_Icon.png";
}