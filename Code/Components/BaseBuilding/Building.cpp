#include "StdAfx.h"
#include "Building.h"
#include "GamePlugin.h"

#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Controller/AIController.h>
#include <Components/Weapons/BaseWeapon.h>
#include <Components/Selectables/UnitAnimation.h>
#include <Components/Managers/ActionManager.h>
#include <Components/Player/Player.h>

#include <Utils/MathUtils.h>

#include <CryGame/IGameFramework.h>
#include <Cry3DEngine/IMaterial.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


namespace
{
	static void RegisterBuildingComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(BuildingComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterBuildingComponent);
}


void BuildingComponent::Initialize()
{
	//AnimationComponent Initialization
	m_pAnimationComponent = m_pEntity->GetComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();

	//trussMsehComponent Initialization
	m_pTrussMeshComponent = m_pEntity->GetComponent<Cry::DefaultComponents::CStaticMeshComponent>();

	//BoxComponent Initialization
	m_pBboxComponent = m_pEntity->GetComponent<Cry::DefaultComponents::CBoxPrimitiveComponent>();


	AABB aabb;
	m_pEntity->GetWorldBounds(aabb);
	Vec3 min = aabb.min;
	Vec3 max = aabb.max;

	f32 width = max.x - min.x;
	f32 height = max.y - min.y;

	//DecalComponent
	m_pDecalComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CDecalComponent>();
	m_pDecalComponent->SetTransformMatrix(Matrix34::Create(Vec3(width + 2, height + 2, 3), IDENTITY, Vec3(0)));
	m_pDecalComponent->SetMaterialFileName("Materials/buildings/building_green_material.mtl");
	m_pDecalComponent->SetSortPriority(50);
	m_pDecalComponent->SetDepth(10);
	m_pDecalComponent->Spawn();
}


Cry::Entity::EventFlags BuildingComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::PhysicsCollision |
		Cry::Entity::EEvent::Reset;
}

void BuildingComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		f32 DeltaTime = event.fParam[0];

		//TODO : hazf beshe
		Build();	

		UpdateMaterial();

		//Timers
		if (m_builtTimePassed < m_timeBetweenBuilds) {
			m_builtTimePassed += 0.5f * DeltaTime;
		}
	}break;
	case Cry::Entity::EEvent::PhysicsCollision: {
		CryLog("collision building");

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

void BuildingComponent::UpdateMaterial()
{
	if (!m_pDecalComponent || bIsPlaced) {
		return;
	}

	if (CanBePlaced()) {
		m_pDecalComponent->SetMaterialFileName("Materials/buildings/building_green_material.mtl");
	}
	else {
		m_pDecalComponent->SetMaterialFileName("Materials/buildings/building_red_material.mtl");
	}
}

void BuildingComponent::Place(Vec3 at)
{
	m_pEntity->SetPos(at);

	bIsPlaced = true;

	m_pEntity->RemoveComponent<Cry::DefaultComponents::CDecalComponent>();

	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0, 0, -5)));

	//Physicalize
	SEntityPhysicalizeParams physParams;
	physParams.type = PE_STATIC;
	physParams.mass = 38000.f;
	m_pEntity->Physicalize(physParams);
}

void BuildingComponent::Build()
{
	if (!m_pAnimationComponent || bIsBuilt || !bIsPlaced) {
		return;
	}

	if (m_currentBuiltAmount < m_pBuildingInfo.m_maxBuildAmount) {
		if (m_builtTimePassed >= m_timeBetweenBuilds) {
			m_currentBuiltAmount += 2.f;
			m_builtTimePassed = 0;

			Vec3 currentPos = m_pAnimationComponent->GetTransformMatrix().GetTranslation();
			currentPos.z += 0.5f;
			m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, currentPos));
		}
	}
	else {
		bIsBuilt = true;
		m_pEntity->RemoveComponent<Cry::DefaultComponents::CStaticMeshComponent>();
		m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0)));
	}
}

void BuildingComponent::SetBuildingInfo(SBuildingInfo buildingInfo)
{
	this->m_pBuildingInfo = buildingInfo;
}

bool BuildingComponent::IsPlaced()
{
	return bIsPlaced;
}

bool BuildingComponent::CanBePlaced()
{
	if (!m_pBboxComponent) {
		return false;
	}

	//TODO:update beshe (performance)  && min / max az koja biad
	AABB aabb;
	m_pEntity->GetWorldBounds(aabb);
	Vec3 min = Vec3(aabb.min.x - 2, aabb.min.y - 2, aabb.min.z);
	Vec3 max = Vec3(aabb.max.x + 6.6f, aabb.max.y + 4, aabb.max.z);
	AABB newAAbb = AABB(min, max);

	IPersistantDebug* pd = gEnv->pGameFramework->GetIPersistantDebug();

	IEntityItPtr entityItPtr = gEnv->pEntitySystem->GetEntityIterator();
	entityItPtr->MoveFirst();
	while (!entityItPtr->IsEnd())
	{
		IEntity* pPlayer = gEnv->pEntitySystem->FindEntityByName(PLAYER_ENTITY_NAME);

		IEntity* other = entityItPtr->Next();
		AABB otherAABB;
		other->GetWorldBounds(otherAABB);
		if (newAAbb.IsIntersectBox(otherAABB) && other != m_pEntity && !other->GetComponent<Cry::DefaultComponents::CEnvironmentProbeComponent>() && other != pPlayer) {
			pd->AddAABB(newAAbb.min, newAAbb.max, ColorF(1, 0, 0), 2);
			return false;
		}
	}
	pd->AddAABB(newAAbb.min, newAAbb.max, ColorF(0, 1, 0), 2);
	return true;
}
