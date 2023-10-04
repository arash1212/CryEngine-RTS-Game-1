#include "StdAfx.h"
#include "AIController.h"
#include "GamePlugin.h"

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterAIControllerComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(AIControllerComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterAIControllerComponent);
}

void AIControllerComponent::Initialize()
{
	m_pCharacterControllerComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
	m_pCharacterControllerComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0, 0, 1)));

	//Navigation component initialization
	m_pNavigationComponent = m_pEntity->GetOrCreateComponent<IEntityNavigationComponent>();
	m_pNavigationComponent->SetNavigationAgentType("MediumSizedCharacters");
	//MovementProperties
	IEntityNavigationComponent::SMovementProperties m_movementProps;
	m_movementProps.normalSpeed = 6.f;
	m_movementProps.minSpeed = 5.5;
	m_movementProps.maxSpeed = 7;
	m_movementProps.lookAheadDistance = 0.5f;
	m_pNavigationComponent->SetMovementProperties(m_movementProps);

	//Collision avoidance
	IEntityNavigationComponent::SCollisionAvoidanceProperties collisionAvoidanceProps;
	collisionAvoidanceProps.radius = 0.1f;
	m_pNavigationComponent->SetCollisionAvoidanceProperties(collisionAvoidanceProps);

	//
	m_moveToPosition = m_pEntity->GetWorldPos();
}

Cry::Entity::EventFlags AIControllerComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void AIControllerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		f32 DeltaTime = event.fParam[0];

		Move(DeltaTime);
	}break;
	case Cry::Entity::EEvent::Reset: {
		m_moveToPosition = m_pEntity->GetWorldPos();

	}break;
	default:
		break;
	}
}

void AIControllerComponent::Move(f32 DeltaTime)
{
	if (m_moveToPosition == ZERO) {
		return;
	}

	m_pNavigationComponent->NavigateTo(m_moveToPosition);
	Vec3 velocity = m_pNavigationComponent->GetRequestedVelocity();

	m_pEntity->SetRotation(Quat::CreateRotationVDir(velocity));
	m_pCharacterControllerComponent->SetVelocity(velocity);
}

void AIControllerComponent::MoveTo(Vec3 position)
{
	if (position == ZERO) {
		return;
	}
	m_moveToPosition = position;
}

bool AIControllerComponent::IsMoving()
{
	return m_pCharacterControllerComponent->IsWalking();
}

bool AIControllerComponent::IsOnGround()
{
	return m_pCharacterControllerComponent->IsOnGround();
}

void AIControllerComponent::SetMoveSpeed(f32 speed)
{
	this->m_moveSpeed = speed;
}

void AIControllerComponent::StopMoving()
{
	this->m_pNavigationComponent->NavigateTo(m_pEntity->GetWorldPos());
}

Vec3 AIControllerComponent::GetVelocity()
{
	return m_pCharacterControllerComponent->GetVelocity();
}
