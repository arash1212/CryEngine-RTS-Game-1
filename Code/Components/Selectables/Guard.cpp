#include "StdAfx.h"
#include "Guard.h"
#include "GamePlugin.h"

#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Controller/AIController.h>
#include <Components/Weapons/BaseWeapon.h>
#include <Components/Selectables/UnitAnimation.h>
#include <Components/Managers/ActionManager.h>
#include <Components/BaseBuilding/Building.h>

#include <Components/Controller/AIController.h>
#include <Components/BaseBuilding/Building.h>

#include <Components/Managers/ResourceManager.h>

#include <Components/Selectables/GuardPost.h>
#include <Components/Selectables/ResourceCollector.h>
#include <Components/Selectables/ResourceStorage.h>

#include <Utils/MathUtils.h>
#include <Utils/EntityUtils.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


namespace
{
	static void RegisterGuardComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(GuardComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterGuardComponent);
}


void GuardComponent::Initialize()
{
	//AIController Initialization
	m_pAIController = m_pEntity->GetOrCreateComponent<AIControllerComponent>();

	//OwnerComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetComponent<OwnerInfoComponent>();
}


Cry::Entity::EventFlags GuardComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void GuardComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];

		EnterAssignedGuardPost();

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

void GuardComponent::EnterAssignedGuardPost()
{
	if (!m_pAssignedGuardPost) {
		return;
	}
	if (bIsEnteredGuardPost) {
		return;
	}
	BuildingComponent* guardPost = m_pAssignedGuardPost->GetComponent<BuildingComponent>();
	if (!guardPost) {
		return;
	}

	f32 distanceToGuardPostEnterance = EntityUtils::GetDistance(m_pEntity->GetWorldPos(), guardPost->GetExitPoint(), nullptr);
	if (distanceToGuardPostEnterance > 1.f) {
		m_pAIController->MoveTo(guardPost->GetExitPoint(), true);
		m_pAIController->LookAtWalkDirection();
	}
	else {
		bIsEnteredGuardPost = true;
	}
}

void GuardComponent::AssignGuardPost(IEntity* guardPost)
{
	GuardPostComponent* pGuardPostComponent = guardPost->GetComponent<GuardPostComponent>();
	if (!pGuardPostComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "GuardComponent : (AssignGuardPost) Selected Entity is not a guardPost.");
		return;
	}
	this->m_pAssignedGuardPost = guardPost;
}

IEntity* GuardComponent::GetAssignedGuardPost()
{
	return m_pAssignedGuardPost;
}

void GuardComponent::CancelAssignedGuardPost()
{
	this->bIsEnteredGuardPost = false;
	this->bIsReachedAssignedPoint = false;
	this->m_pAssignedGuardPost = nullptr;
	this->m_pAIController->StopMoving();
}

bool GuardComponent::HasEnteredGuardPost()
{
	if (!m_pAssignedGuardPost) {
		return false;
	}

	return bIsEnteredGuardPost;
}

void GuardComponent::SetHasEnteredGuardPost(bool hasEntered)
{
	this->bIsEnteredGuardPost = hasEntered;
}