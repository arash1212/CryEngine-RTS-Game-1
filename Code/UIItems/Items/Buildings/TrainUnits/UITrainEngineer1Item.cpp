#include "StdAfx.h"
#include "UITrainEngineer1Item.h"
#include "GamePlugin.h"

#include <CryEntitySystem/IEntitySystem.h>
#include <Components/BaseBuilding/Building.h>
#include <Components/Selectables/Cost.h>
#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/ResourceManager.h>
#include <Components/Selectables/Units/Engineer1Unit.h>

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
	OwnerInfoComponent* ownerComponent = m_pEntity->GetComponent<OwnerInfoComponent>();
	if (!ownerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UITrainEngineer1Item : (Execute) ownerComponent is null !");
		return;
	}
	IEntity* owner = ownerComponent->GetOwner();
	if (!owner) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UITrainEngineer1Item : (Execute) owner is null !");
		return;
	}
	ResourceManagerComponent* resouceManager = owner->GetComponent<ResourceManagerComponent>();
	if (!resouceManager) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UITrainEngineer1Item : (Execute) resoucwManager is null !");
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

	if (!resouceManager->RequsetResources(Engineer1UnitComponent::GetDescription().cost)) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UITrainEngineer1Item : (Execute) Not enough resources to train Engineer1 unit !");
		return;
	}
	if (actionManager) {
		CryLog("engineer action added");
		actionManager->AddAction(new TrainEngineer1Action(m_pEntity, building->GetExitPoint()));
	}
}

string UITrainEngineer1Item::GetImagePath()
{
	return Engineer1UnitComponent::GetDescription().sIcon;
}

SUIItemDescription UITrainEngineer1Item::GetDescrption()
{
	return SUIItemDescription(Engineer1UnitComponent::GetDescription().cost, Engineer1UnitComponent::GetDescription().sBuyDescription);
}