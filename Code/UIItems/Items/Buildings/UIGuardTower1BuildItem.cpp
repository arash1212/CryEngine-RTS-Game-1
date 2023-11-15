#include "StdAfx.h"
#include "UIGuardTower1BuildItem.h"
#include "GamePlugin.h"

#include <Components/Player/Player.h>
#include <CryEntitySystem/IEntitySystem.h>
#include <Components/BaseBuilding/BaseBuilding.h>
#include <Components/BaseBuilding/Buildings/GuardTower1Building.h>
#include <Components/Info/OwnerInfo.h>

#include <Components/Managers/ActionManager.h>
#include <Components/Managers/ResourceManager.h>
#include <Components/Selectables/Cost.h>

UIGuardTower1BuildItem::UIGuardTower1BuildItem(IEntity* entity)
{
	this->m_pEntity = entity;
}

void UIGuardTower1BuildItem::Execute()
{
	if (!m_pEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIGuardTower1BuildItem : (Execute) m_pEntity is null !");
		return;
	}

	IEntity* m_pPlayerEntity = gEnv->pEntitySystem->FindEntityByName(PLAYER_ENTITY_NAME);
	if (!m_pPlayerEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIGuardTower1BuildItem : (Execute) m_pPlayerEntity is null !");
		return;
	}
	OwnerInfoComponent* pOwnerInfo = m_pEntity->GetComponent<OwnerInfoComponent>();
	if (!pOwnerInfo) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIGuardTower1BuildItem : (Execute) pOwnerInfo | OwnerEntity is null !");
		return;
	}
	IEntity* ownerEntity = pOwnerInfo->GetOwner();
	if (!ownerEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIGuardTower1BuildItem : (Execute) ownerEntity is null !");
		return;
	}
	ResourceManagerComponent* resourceManager = ownerEntity->GetComponent<ResourceManagerComponent>();
	if (!ownerEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIGuardTower1BuildItem : (Execute) resourceManager is null !");
		return;
	}

	if (!resourceManager->RequsetResources(GaurdTower1BuildingComponent::GetDescription().price)) {
		return;
	}

	ActionManagerComponent* actionManager = m_pEntity->GetComponent<ActionManagerComponent>();
	if (actionManager) {
		BaseBuildingComponent* baseBuildingComponent = m_pPlayerEntity->GetComponent<BaseBuildingComponent>();
		IEntity* pBuildingEntity = baseBuildingComponent->AssignBuilding(nullptr);
		if (pBuildingEntity) {
			pBuildingEntity->GetOrCreateComponent<GaurdTower1BuildingComponent>();
		}
	}
}

string UIGuardTower1BuildItem::GetImagePath()
{
	return  GaurdTower1BuildingComponent::GetDescription().sIcon;
}

SDescription UIGuardTower1BuildItem::GetDescrption()
{
	return GaurdTower1BuildingComponent::GetDescription();
}