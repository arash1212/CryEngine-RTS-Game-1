#include "StdAfx.h"
#include "AIController.h"
#include "GamePlugin.h"

#include <CryEntitySystem/IEntitySystem.h>
#include <CryAISystem/IAISystem.h>
#include <CryAISystem/INavigationSystem.h>
#include <CryAISystem/NavigationSystem/INavMeshQueryFilter.h>

#include <Components/Managers/UnitStateManager.h>

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
	//CharacterController component initialization
	m_pCharacterControllerComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
	m_pCharacterControllerComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0, 0, 1)));

	//Navigation component initializations
	m_pNavigationComponent = m_pEntity->GetOrCreateComponent<IEntityNavigationComponent>();
	m_pNavigationComponent->SetNavigationAgentType("MediumSizedCharacters");
	//MovementProperties
	IEntityNavigationComponent::SMovementProperties m_movementProps;
	m_movementProps.normalSpeed = 4.f;
	m_movementProps.minSpeed = 3.5;
	m_movementProps.maxSpeed = 7;
	m_movementProps.lookAheadDistance = 0.5f;
	m_pNavigationComponent->SetMovementProperties(m_movementProps);

	//Collision avoidance
	IEntityNavigationComponent::SCollisionAvoidanceProperties collisionAvoidanceProps;
	collisionAvoidanceProps.radius = 0.1f;
	m_pNavigationComponent->SetCollisionAvoidanceProperties(collisionAvoidanceProps);

	//StateManager initialization
	m_pStateManager = m_pEntity->GetComponent<UnitStateManagerComponent>();

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
	if (!m_pStateManager) {
		CryLog("AIControllerComponent : (Move) UnitStateManagerComponent does not exist on entity !");
		return;
	}
	if (m_moveToPosition == ZERO) {
		return;
	}

	m_pNavigationComponent->NavigateTo(m_moveToPosition);
	Vec3 velocity = m_pNavigationComponent->GetRequestedVelocity();
	m_pCharacterControllerComponent->SetVelocity(velocity.normalized() * m_pStateManager->GetCurrentSpeed());
}

void AIControllerComponent::MoveTo(Vec3 position, bool run)
{
	if (position == ZERO) {
		return;
	}
	if (!m_pStateManager) {
		CryLog("AIControllerComponent : (MoveTo) UnitStateManagerComponent does not exist on entity !");
		return;
	}

	/*
	//Set state to run
	if (run) {
		m_pStateManager->SetStance(EUnitStance::RUNNING);
	}*/

	m_moveToPosition = this->SnapToNavmesh(position);
}

void AIControllerComponent::LookAt(Vec3 position)
{
	if (position == ZERO) {
		return;
	}

	Vec3 dir = position - m_pEntity->GetWorldPos();
	dir.z = 0;
	m_pEntity->SetRotation(Quat::CreateRotationVDir(dir));
}

f32 AIControllerComponent::AngleTo(Vec3 position)
{
	Vec3 dir = position - m_pEntity->GetWorldPos();
	Vec3 forwardVector = m_pEntity->GetForwardDir().normalized();
	float dot = forwardVector.dot(dir);
	return crymath::acos(dot);
}

Vec3 AIControllerComponent::SnapToNavmesh(Vec3 point)
{
	NavigationAgentTypeID agentTypeId = NavigationAgentTypeID::TNavigationID(1);
	NavigationMeshID navMeshId = gEnv->pAISystem->GetNavigationSystem()->FindEnclosingMeshID(agentTypeId, point);
	MNM::SOrderedSnappingMetrics snappingMetrics;
	snappingMetrics.CreateDefault();
	SAcceptAllQueryTrianglesFilter filter;
	MNM::SPointOnNavMesh pointOnNavMesh = gEnv->pAISystem->GetNavigationSystem()->SnapToNavMesh(agentTypeId, point, snappingMetrics, &filter, &navMeshId);
	return pointOnNavMesh.GetWorldPosition();
}

void AIControllerComponent::StopMoving()
{
	this->m_moveToPosition = m_pEntity->GetWorldPos();
	this->m_pCharacterControllerComponent->SetVelocity(ZERO);
	//this->MoveTo(m_pEntity->GetWorldPos(), false);
}

void AIControllerComponent::LookAtWalkDirection()
{
	m_pEntity->SetRotation(Quat::CreateRotationVDir(m_pNavigationComponent->GetRequestedVelocity()));
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

Vec3 AIControllerComponent::GetVelocity()
{
	return m_pCharacterControllerComponent->GetVelocity();
}

Cry::DefaultComponents::CCharacterControllerComponent* AIControllerComponent::GetCharacterController()
{
	return m_pCharacterControllerComponent;
}
