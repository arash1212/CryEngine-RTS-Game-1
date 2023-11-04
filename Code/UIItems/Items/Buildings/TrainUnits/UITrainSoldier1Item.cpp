#include "StdAfx.h"
#include "UITrainSoldier1Item.h"
#include "GamePlugin.h"

#include <CryEntitySystem/IEntitySystem.h>
#include <Components/BaseBuilding/Building.h>
#include <Components/Selectables/Cost.h>
#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/ResourceManager.h>
#include <Components/Selectables/Units/Soldier1Unit.h>

#include <Actions/Train/TrainSoldier1Action.h>
#include <Components/Managers/ActionManager.h>

UITrainSoldier1Item::UITrainSoldier1Item(IEntity* entity)
{
	this->m_pEntity = entity;
}

void UITrainSoldier1Item::Execute()
{
	if (!m_pEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UITrainSoldier1Item : (Execute) m_pEntity is null !");
		return;
	}
	OwnerInfoComponent* ownerComponent = m_pEntity->GetComponent<OwnerInfoComponent>();
	if (!ownerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UITrainSoldier1Item : (Execute) ownerComponent is null !");
		return;
	}
	IEntity* owner = ownerComponent->GetOwner();
	if (!owner) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UITrainSoldier1Item : (Execute) owner is null !");
		return;
	}
	ResourceManagerComponent* resouceManager = owner->GetComponent<ResourceManagerComponent>();
	if (!resouceManager) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UITrainSoldier1Item : (Execute) resoucwManager is null !");
		return;
	}
	BuildingComponent* building = m_pEntity->GetComponent<BuildingComponent>();
	if (!building) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UITrainSoldier1Item : (Execute) buildingComponent is null !");
		return;
	}
	ActionManagerComponent* actionManager = m_pEntity->GetComponent<ActionManagerComponent>();
	if (!actionManager) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UITrainSoldier1Item : (Execute) actionManager is null !");
		return;
	}

	if (!resouceManager->RequsetResources(Soldier1UnitComponent::GetCost())) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UITrainSoldier1Item : (Execute) Not enough resources to train Soldier1 unit !");
		return;
	}
	if (actionManager) {
		actionManager->AddAction(new TrainSoldier1Action(m_pEntity, building->GetExitPoint()));
	}
}

string UITrainSoldier1Item::GetImagePath()
{
	return "soldier_1_icon.png";
}