#include "StdAfx.h"
#include "UnitStateManager.h"
#include "GamePlugin.h"

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
		//CryLog("%s UnitStateManagerComponent : (UpdateState) characterControllerComponent is null", m_pEntity->GetName());
		return;
	}

	if (m_pCharacterControllerComponent->IsOnGround() && !m_pCharacterControllerComponent->IsWalking()) {
		m_pUnitState = EUnitState::IDLE;
	}
	if (m_pCharacterControllerComponent->IsOnGround() && m_pCharacterControllerComponent->IsWalking() && m_currentSpeed == m_walkSpeed) {
		m_pUnitState = EUnitState::WALK;
	}
	if (m_pCharacterControllerComponent->IsOnGround() && m_pCharacterControllerComponent->IsWalking() && m_currentSpeed == m_runSpeed) {
		m_pUnitState = EUnitState::RUN;
	}
}

EUnitState UnitStateManagerComponent::GetState()
{
	return m_pUnitState;
}

EUnitStance UnitStateManagerComponent::GetStance()
{
	return m_pUnitStance;
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

void UnitStateManagerComponent::SetCrouchSpeed(f32 crouchSpeed)
{
	this->m_crouchSpeed = crouchSpeed;
}

void UnitStateManagerComponent::SetRunSpeed(f32 runSpeed)
{
	this->m_runSpeed = runSpeed;
}

void UnitStateManagerComponent::SetProneSpeed(f32 proneSpeed)
{
	this->m_proneSpeed = proneSpeed;
}

void UnitStateManagerComponent::SetCurrentSpeed(f32 currentSpeed)
{
	this->m_currentSpeed = currentSpeed;
}

f32 UnitStateManagerComponent::GetCurrentSpeed()
{
	return this->m_currentSpeed;
}
