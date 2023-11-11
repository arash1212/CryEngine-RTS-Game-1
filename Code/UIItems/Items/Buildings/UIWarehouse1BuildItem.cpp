#include "StdAfx.h"
#include "UIWarehouse1BuildItem.h"
#include "GamePlugin.h"

#include <Components/Player/Player.h>
#include <CryEntitySystem/IEntitySystem.h>
#include <Components/BaseBuilding/BaseBuilding.h>
#include <Components/BaseBuilding/Buildings/Warehouse1Building.h>
#include <Components/Info/OwnerInfo.h>

#include <Components/Managers/ActionManager.h>
#include <Components/Managers/ResourceManager.h>
#include <Components/Selectables/Cost.h>

UIWarehouse1BuildItem::UIWarehouse1BuildItem(IEntity* entity)
{
	this->m_pEntity = entity;
}

void UIWarehouse1BuildItem::Execute()
{
	if (!m_pEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UICancelAction : (Execute) m_pEntity is null !");
		return;
	}

	IEntity* m_pPlayerEntity = gEnv->pEntitySystem->FindEntityByName(PLAYER_ENTITY_NAME);
	if (!m_pPlayerEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UICancelAction : (Execute) m_pPlayerEntity is null !");
		return;
	}
	OwnerInfoComponent* pOwnerInfo = m_pEntity->GetComponent<OwnerInfoComponent>();
	if (!pOwnerInfo) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UICancelAction : (Execute) pOwnerInfo | OwnerEntity is null !");
		return;
	}
	IEntity* ownerEntity = pOwnerInfo->GetOwner();
	if (!ownerEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UICancelAction : (Execute) ownerEntity is null !");
		return;
	}
	ResourceManagerComponent* resourceManager = ownerEntity->GetComponent<ResourceManagerComponent>();
	if (!ownerEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UICancelAction : (Execute) resourceManager is null !");
		return;
	}

	if (!resourceManager->RequsetResources(Warehouse1BuildingComponent::GetDescription().price)) {
		return;
	}

	ActionManagerComponent* actionManager = m_pEntity->GetComponent<ActionManagerComponent>();
	if (actionManager) {
		BaseBuildingComponent* baseBuildingComponent = m_pPlayerEntity->GetComponent<BaseBuildingComponent>();
		IEntity* pBuildingEntity = baseBuildingComponent->AssignBuilding(nullptr);
		if (pBuildingEntity) {
			pBuildingEntity->GetOrCreateComponent<Warehouse1BuildingComponent>();
		}
	}
}

string UIWarehouse1BuildItem::GetImagePath()
{
	return Warehouse1BuildingComponent::GetDescription().sIcon;
}


SDescription UIWarehouse1BuildItem::GetDescrption()
{
	return Warehouse1BuildingComponent::GetDescription();
}