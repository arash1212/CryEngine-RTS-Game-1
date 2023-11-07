#include "StdAfx.h"
#include "BuildingBuildAction.h"
#include "GamePlugin.h"

#include <Utils/EntityUtils.h>

#include <Components/BaseBuilding/Building.h>
#include <Components/Selectables/Engineer.h>
#include <Components/Controller/AIController.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Selectables/UnitAnimation.h>
#include <Components/Managers/ResourceManager.h>
#include <Components/Selectables/Cost.h>

BuildingBuildAction::BuildingBuildAction(IEntity* entity)
{
	this->m_pEntity = entity;
	this->m_pBuildingComponent = entity->GetComponent<BuildingComponent>();
}

void BuildingBuildAction::Execute()
{
	if (!m_pBuildingComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "BuildingBuildAction : (Execute) m_pBuildingComponent cannot be null.Action canceled.");
		Cancel();
		return;
	}
	if (this->m_pBuildingComponent->IsBuilt()) {
		bIsDone = true;
		return;
	}

	//Baraye progress bar sakhte shod felan

}

void BuildingBuildAction::Cancel()
{
	bIsDone = true;
}

bool BuildingBuildAction::IsDone()
{
	return this->m_pBuildingComponent->IsBuilt() || bIsDone;
}

f32 BuildingBuildAction::GetProgressAmount()
{
	return this->m_pBuildingComponent->GetCurrentBuildAmount();
}

f32 BuildingBuildAction::GetMaxProgressAmount()
{
	return this->m_pBuildingComponent->GetBuildingInfos().m_maxBuildAmount;
}