#include "StdAfx.h"
#include "Worker.h"
#include "GamePlugin.h"

#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Controller/AIController.h>
#include <Components/Weapons/BaseWeapon.h>
#include <Components/Selectables/UnitAnimation.h>
#include <Components/Managers/ActionManager.h>
#include <Components/BaseBuilding/Building.h>

#include <Components/Selectables/Workplace.h>
#include <Components/Controller/AIController.h>

#include <Utils/MathUtils.h>
#include <Utils/EntityUtils.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


namespace
{
	static void RegisterWorkerComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(WorkerComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterWorkerComponent);
}


void WorkerComponent::Initialize()
{
	//AIController Initializations
	m_pAIController = m_pEntity->GetOrCreateComponent<AIControllerComponent>();
}


Cry::Entity::EventFlags WorkerComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void WorkerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];

		EnterAssignedWorkplace();

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

void WorkerComponent::EnterAssignedWorkplace()
{
	if (!m_pAssignedWorkplace) {
		return;
	}
	if (bIsEnteredWorkplace) {
		return;
	}
	BuildingComponent* workplace = m_pAssignedWorkplace->GetComponent<BuildingComponent>();
	if (!workplace) {
		return;
	}

	f32 distanceToWorkplaceEnterance = EntityUtils::GetDistance(m_pEntity->GetWorldPos(), workplace->GetExitPoint(), nullptr);
	if (distanceToWorkplaceEnterance > 1.f) {
		m_pAIController->MoveTo(workplace->GetExitPoint(), true);
		m_pAIController->LookAtWalkDirection();
	}
	else {
		bIsEnteredWorkplace = true;
	}
}

void WorkerComponent::AssignWorkplace(IEntity* workplace)
{
	WorkplaceComponent* workplaceComponent = workplace->GetComponent<WorkplaceComponent>();
	if (!workplaceComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "WorkerComponent : (AssignWorkplace) Selected Entity is not a workplace.");
		return;
	}
	this->m_pAssignedWorkplace = workplace;
}

void WorkerComponent::CancelAssignedWorkplace()
{
	this->bIsEnteredWorkplace = false;
	this->bIsReachedAssignedPoint = false;
	this->m_pAssignedWorkplace = nullptr;
	this->m_pAIController->StopMoving();
}

bool WorkerComponent::HasEnteredWorkplace()
{
	if (!m_pAssignedWorkplace) {
		return false;
	}

	return bIsEnteredWorkplace;
}

void WorkerComponent::SetHasEnteredWorkplace(bool hasEntered)
{
	this->bIsEnteredWorkplace = hasEntered;
}

bool WorkerComponent::HasReachedAssignedPoint()
{
	return bIsReachedAssignedPoint;
}
