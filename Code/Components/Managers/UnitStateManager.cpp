#include "StdAfx.h"
#include "UnitStateManager.h"
#include "GamePlugin.h"

#include <Components/Selectables/Units/BaseUnit.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterUnitStateManagerComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(UnitStateManagerComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterUnitStateManagerComponent);
}

void UnitStateManagerComponent::Initialize()
{
}

Cry::Entity::EventFlags UnitStateManagerComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void UnitStateManagerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];

		UpdateState();

	}break;
	case Cry::Entity::EEvent::Reset: {


	}break;
	default:
		break;
	}
}

void UnitStateManagerComponent::UpdateState()
{
	if (!m_pCharacterControllerComponent) {
		CryLog("%s UnitStateManagerComponent : (UpdateState) characterControllerComponent is null", m_pEntity->GetName());
		return;
	}

	BaseUnitComponent* unit = m_pEntity->GetComponent<BaseUnitComponent>();
	if (!unit) {
		return;
	}

	if (m_pCharacterControllerComponent->IsOnGround() && !m_pCharacterControllerComponent->IsWalking()) {
		m_pUnitState = EUnitState::IDLE;
	}
	else if (m_pCharacterControllerComponent->IsOnGround() && m_pCharacterControllerComponent->IsWalking() && unit->GetCurrentSpeed() == m_walkSpeed) {
		m_pUnitState = EUnitState::WALK;
	}
	else if (m_pCharacterControllerComponent->IsOnGround() && m_pCharacterControllerComponent->IsWalking() && unit->GetCurrentSpeed() == m_runSpeed) {
		m_pUnitState = EUnitState::RUN;
	}
}

EUnitState UnitStateManagerComponent::GetState()
{
	return m_pUnitState;
}

void UnitStateManagerComponent::SetStance(EUnitStance stance)
{
	this->m_pUnitStance = stance;
}

void UnitStateManagerComponent::SetCharacterController(Cry::DefaultComponents::CCharacterControllerComponent* characterControllerComponent)
{
	this->m_pCharacterControllerComponent = characterControllerComponent;
}

void UnitStateManagerComponent::SetWalkSpeed(f32 walkSpeed)
{
	this->m_walkSpeed = walkSpeed;
}

void UnitStateManagerComponent::SetRunSpeed(f32 runSpeed)
{
	this->m_runSpeed = runSpeed;
}
