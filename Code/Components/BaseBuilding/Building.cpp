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

#include <Components/Selectables/Selectable.h>
#include <UIItems/IBaseUIItem.h>
#include <UIItems/Items/UICancelItem.h>
#include <UIItems/Items/UIChangeStanceItem.h>
#include <UIItems/Items/Buildings/UIHQ1BuildItem.h>

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

	//DecalComponent(Placement) Initialization
	m_pDecalComponent = m_pEntity->CreateComponent<Cry::DefaultComponents::CDecalComponent>();
	m_pDecalComponent->SetTransformMatrix(Matrix34::Create(Vec3(width + 2, height + 2, 3), IDENTITY, Vec3(0)));
	m_pDecalComponent->SetMaterialFileName(BUILDING_PLACEMENT_GREEN_DECAL_MATERIAL);
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
		m_pDecalComponent->SetMaterialFileName(BUILDING_PLACEMENT_GREEN_DECAL_MATERIAL);
	}
	else {
		m_pDecalComponent->SetMaterialFileName(BUILDING_PLACEMENT_RED_DECAL_MATERIAL);
	}
}

void BuildingComponent::Place(Vec3 at)
{
	m_pEntity->SetPos(at);

	bIsPlaced = true;

	m_pEntity->RemoveComponent<Cry::DefaultComponents::CDecalComponent>();

	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0, 0, -5)));

	PlacementCheck();

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

	//
	if (m_currentBuiltAmount < m_pBuildingInfo.m_maxBuildAmount) {
		if (m_builtTimePassed >= m_timeBetweenBuilds) {
			m_currentBuiltAmount += 2.f;
			m_builtTimePassed = 0;

			Vec3 currentPos = m_pAnimationComponent->GetTransformMatrix().GetTranslation();
			currentPos.z += 0.5f;
			m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, currentPos));
		}
	}

	//if is built
	else {
		bIsBuilt = true;
		m_pEntity->RemoveComponent<Cry::DefaultComponents::CStaticMeshComponent>();
		m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0)));

		//Add SelectableComponent 
		m_pSelectableComponent = m_pEntity->GetOrCreateComponent<SelectableComponent>();
		m_pSelectableComponent->SetDecalSize(Vec3(8));
		m_pSelectableComponent->DeSelect();
		//Add UI items
		for (IBaseUIItem* item : m_pAllUIItems) {
			m_pSelectableComponent->AddUIItem(item);
		}
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

	AABB aabb;
	m_pEntity->GetWorldBounds(aabb);
	IPersistantDebug* pd = gEnv->pGameFramework->GetIPersistantDebug();
	IEntityItPtr entityItPtr = gEnv->pEntitySystem->GetEntityIterator();

	//DynArray<IPhysicalEntity**> pEntities;
	//CryLog("size %i : ", gEnv->pPhysicalWorld->GetEntitiesInBox(aabb.min, aabb.max, *pEntities.data(), ent_rigid | ent_sleeping_rigid | ent_water | ent_living | ent_static));

	entityItPtr->MoveFirst();
	while (!entityItPtr->IsEnd())
	{
		IEntity* pPlayer = gEnv->pEntitySystem->FindEntityByName(PLAYER_ENTITY_NAME);
		IEntity* other = entityItPtr->Next();
		if (other == m_pEntity || other == pPlayer) {
			continue;
		}

		AABB otherAABB;
		other->GetWorldBounds(otherAABB);
		if (aabb.IsIntersectBox(otherAABB) && !other->GetComponent<Cry::DefaultComponents::CEnvironmentProbeComponent>() ) {
			pd->AddAABB(aabb.min, aabb.max, ColorF(1, 0, 0), 2);
			return false;
		}
	}
	pd->AddAABB(aabb.min, aabb.max, ColorF(0, 1, 0), 2);
	return true;
}

void BuildingComponent::SetPathToTrussMesh(string path)
{
	this->m_pathToTrussMesh = path;
}

void BuildingComponent::AddUIItem(IBaseUIItem* item)
{
	this->m_pAllUIItems.append(item);
}

void BuildingComponent::PlacementCheck()
{
	m_pTrussMeshComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CStaticMeshComponent>();
	m_pTrussMeshComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0)));
	m_pTrussMeshComponent->SetFilePath(m_pathToTrussMesh);
	m_pTrussMeshComponent->LoadFromDisk();
	m_pTrussMeshComponent->ResetObject();
}
