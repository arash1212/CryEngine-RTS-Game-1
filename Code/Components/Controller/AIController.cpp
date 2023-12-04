#include "StdAfx.h"
#include "AIController.h"
#include "GamePlugin.h"

#include <CryEntitySystem/IEntitySystem.h>
#include <CryAISystem/IAISystem.h>
#include <CryAISystem/INavigationSystem.h>
#include <CryAISystem/NavigationSystem/INavMeshQueryFilter.h>

#include <Components/Managers/UnitStateManager.h>
#include <Utils/MathUtils.h>

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
	m_pCharacterControllerComponent->SetTransformMatrix(Matrix34::Create(Vec3(0.01f), IDENTITY, Vec3(0, 0, 1)));

	//Navigation component initializations
	m_pNavigationComponent = m_pEntity->GetOrCreateComponent<IEntityNavigationComponent>();
	m_pNavigationComponent->SetNavigationAgentType("MediumSizedCharacters");
	//MovementProperties
	IEntityNavigationComponent::SMovementProperties m_movementProps;
	m_movementProps.normalSpeed = 4.f;
	m_movementProps.minSpeed = 3.5;
	m_movementProps.maxSpeed = 5;
	m_movementProps.lookAheadDistance = 0.1f;
	m_movementProps.bStopAtEnd = true;
	m_movementProps.maxDeceleration = 12;
	m_pNavigationComponent->SetMovementProperties(m_movementProps);

	//Collision avoidance
	IEntityNavigationComponent::SCollisionAvoidanceProperties collisionAvoidanceProps;
	collisionAvoidanceProps.radius = 0.2f;
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

		if (bIsStopped) {
			m_moveToPosition = m_pEntity->GetWorldPos();
		}
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
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "AIControllerComponent : (Move) UnitStateManagerComponent does not exist on entity !");
		return;
	}
	if (m_moveToPosition == ZERO) {
		return;
	}

//	m_pNavigationComponent->NavigateTo(m_moveToPosition);
	Vec3 velocity = m_pNavigationComponent->GetRequestedVelocity();
	m_pCharacterControllerComponent->SetVelocity(velocity.normalized() * m_pStateManager->GetCurrentSpeed());
}

bool AIControllerComponent::MoveTo(Vec3 position, bool run)
{
	if (position == ZERO) {
		return false;
	}
	if (!m_pStateManager) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "AIControllerComponent : (MoveTo) UnitStateManagerComponent does not exist on entity !");
		return false;
	}
	if (!m_pNavigationComponent->IsDestinationReachable(position)) {
		//CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "AIControllerComponent : (MoveTo) Destination is not reachable !");
		//return false;
	}
	if (run) {
		m_pStateManager->SetStance(EUnitStance::RUNNING);
	}

	//this->SnapToNavmesh() ?
	m_moveToPosition = this->SnapToNavmesh(position);
	m_pNavigationComponent->NavigateTo(m_moveToPosition);

	bIsStopped = false;
	return true;
}

void AIControllerComponent::LookAt(Vec3 position)
{
	if (position == ZERO) {
		return;
	}

	Vec3 dir = position - m_pEntity->GetWorldPos();
	dir.z = 0;
	m_pEntity->SetRotation(Quat::CreateRotationVDir(dir.GetNormalizedSafe()));
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
	//this->m_moveToPosition = m_pEntity->GetWorldPos();
	//this->m_pCharacterControllerComponent->SetVelocity(ZERO);
	//this->m_pCharacterControllerComponent->ChangeVelocity(ZERO, Cry::DefaultComponents::CCharacterControllerComponent::EChangeVelocityMode::SetAsTarget);
	//this->m_pStateManager->SetStance(EUnitStance::WALKING);
	//this->MoveTo(m_pEntity->GetWorldPos(), false);
	bIsStopped = true;
}

void AIControllerComponent::LookAtWalkDirection()
{
	m_pEntity->SetRotation(Quat::CreateRotationVDir(m_pNavigationComponent->GetRequestedVelocity().GetNormalizedSafe()));
}

Vec3 AIControllerComponent::GetRandomPointInsideTriangle(Triangle t)
{
	f32 r1 = crymath::sqrt(MathUtils::GetRandomFloat(0.f, 1.f));
	f32 r2 = MathUtils::GetRandomFloat(0.f, 1.f);
	f32 m1 = 1 - r1;
	f32 m2 = r1 * (1 - r2);
	f32 m3 = r2 * r1;

	Vec3 p1 = t.v0;
	Vec3 p2 = t.v1;
	Vec3 p3 = t.v2;
	return (m1 * p1) + (m2 * p2) + (m3 * p3);
}


Vec3 AIControllerComponent::GetRandomPointOnNavmesh(float MaxDistance, Vec3 Around)
{
	MNM::TriangleIDArray resultArray;
	DynArray<Vec3> resultPositions;

	NavigationAgentTypeID agentTypeId = NavigationAgentTypeID::TNavigationID(1);
	NavigationMeshID navMeshId = gEnv->pAISystem->GetNavigationSystem()->FindEnclosingMeshID(agentTypeId, Around);

	//get Triangles
	const MNM::INavMesh* navMesh = gEnv->pAISystem->GetNavigationSystem()->GetMNMNavMesh(navMeshId);
	if (!navMesh) {
		return m_pEntity->GetWorldPos();
	}

	const Vec3 triggerBoxSize = Vec3(20, 20, 20);
	MNM::aabb_t	 aabb = MNM::aabb_t(triggerBoxSize * -15.5f, triggerBoxSize * 15.5f);
	MNM::TriangleIDArray triangleIDArray = navMesh->QueryTriangles(aabb);
	if (triangleIDArray.size() <= 0 || !triangleIDArray[0].IsValid() || !navMeshId.IsValid() || !agentTypeId.IsValid()) {
		return m_pEntity->GetWorldPos();
	}

	for (int32 i = 0; i < triangleIDArray.size(); i++) {
		int32 j = 0;
		Triangle triangle;
		gEnv->pAISystem->GetNavigationSystem()->GetTriangleVertices(navMeshId, triangleIDArray[i], triangle);
		//while (j <= 5) {
		j++;
		Vec3 point = GetRandomPointInsideTriangle(triangle);
		//if (IsPointVisibleFrom(agentTypeId, point, Around->GetWorldPos())) {
			resultPositions.append(point);
		//}
	}
 
	int32 max = resultPositions.size();
	int32 min = 0;
	int32 range = max - min + 1;
	int random = rand() % range + min;
	Vec3 resultPos = resultPositions[random];

	f32 resultMax = MaxDistance - 2;
	f32 resultMin = 3;
	Vec3 Dir = resultPos - Around;

	resultPos = Around + Dir.normalize() * (resultMin + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / resultMax - resultMin)));

	MNM::SOrderedSnappingMetrics snappingMetrics;
	snappingMetrics.CreateDefault();
	SAcceptAllQueryTrianglesFilter filter;
	MNM::SPointOnNavMesh pointOnNavMesh = gEnv->pAISystem->GetNavigationSystem()->SnapToNavMesh(agentTypeId, resultPos, snappingMetrics, &filter, &navMeshId);
	return pointOnNavMesh.GetWorldPosition();
}

bool AIControllerComponent::IsDestinationReachable(Vec3 position)
{
	return this->m_pNavigationComponent->IsDestinationReachable(position);
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
