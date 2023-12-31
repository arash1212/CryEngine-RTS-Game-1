#include "StdAfx.h"
#include "Building.h"
#include "GamePlugin.h"

#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/ActionManager.h>
#include <Components/Player/Player.h>

#include <Actions/Buildings/BuildingBuildAction.h>

#include <Components/Selectables/Selectable.h>
#include <UIItems/IBaseUIItem.h>
#include <UIItems/Items/UICancelItem.h>
#include <UIItems/Items/UIChangeStanceItem.h>
#include <UIItems/Items/Buildings/UIHQ1BuildItem.h>
#include <Components/Selectables/Workplace.h>

#include <CryEntitySystem/IEntitySystem.h>
#include <CryAISystem/INavigationSystem.h>
#include <CryAISystem/NavigationSystem/INavigationUpdatesManager.h>

#include <Utils/MathUtils.h>

#include <CryGame/IGameFramework.h>
#include <Cry3DEngine/IMaterial.h>
#include <Components/Selectables/Cost.h>

#include <Components/Selectables/Health.h>
#include <Components/Selectables/Visibility.h>

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
		//f32 DeltaTime = event.fParam[0];

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
	if (!m_pSkinAttachment || !m_pDecalComponent || bIsPlaced) {
		return;
	}

	IAttachmentObject* pAttachmentObject = m_pSkinAttachment->GetIAttachmentObject();
	if (!pAttachmentObject) {
		return;
	}

	if (CanBePlaced()) {
		m_pDecalComponent->SetMaterialFileName(BUILDING_PLACEMENT_GREEN_DECAL_MATERIAL);

		IMaterial* m_pGreenMaterial = gEnv->p3DEngine->GetMaterialManager()->LoadMaterial(BUILDING_PLACEMENT_GREEN_MATERIAL);
		pAttachmentObject->SetReplacementMaterial(m_pGreenMaterial);
	}
	else {
		m_pDecalComponent->SetMaterialFileName(BUILDING_PLACEMENT_RED_DECAL_MATERIAL);

		IMaterial* m_pRedMaterial = gEnv->p3DEngine->GetMaterialManager()->LoadMaterial(BUILDING_PLACEMENT_RED_MATERIAL);
		pAttachmentObject->SetReplacementMaterial(m_pRedMaterial);
	}
}

void BuildingComponent::Place(Vec3 at)
{
	if (!CanBePlaced()) {
		return;
	}

	bIsPlaced = true;
	m_pEntity->SetPos(at);

	//Remove placement decal 
	m_pEntity->RemoveComponent<Cry::DefaultComponents::CDecalComponent>();

	//Move building mesh down after placement
	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0, 0, -5)));

	//PlaceTruss
	m_pTrussMeshComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CStaticMeshComponent>();
	m_pTrussMeshComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0)));
	m_pTrussMeshComponent->SetFilePath(m_pathToTrussMesh);
	m_pTrussMeshComponent->LoadFromDisk();
	m_pTrussMeshComponent->ResetObject();

	//Physicalize
	SEntityPhysicalizeParams physParams;
	physParams.type = PE_STATIC;
	physParams.mass = 38000.f;
	m_pEntity->Physicalize(physParams);

	//Update Material
	m_pSkinAttachment->GetIAttachmentObject()->SetReplacementMaterial(m_pDefaultMaterial);

	AABB aabb;
	m_pEntity->GetWorldBounds(aabb);
	gEnv->pAISystem->GetNavigationSystem()->GetUpdateManager()->WorldChanged(aabb);

	//HealthComponent Initialization
	m_pHealthComponent = m_pEntity->GetOrCreateComponent<HealthComponent>();
	m_pHealthComponent->SetMaxHealth(m_maxHealth);

	//Build Action
	m_pActionManagerComponent->AddAction(new BuildingBuildAction(m_pEntity));

	//VisibilityComponent Initialization
	m_pVisibilityComponent = m_pEntity->GetOrCreateComponent<VisibilityComponent>();
}

void BuildingComponent::Build()
{
	if (!m_pAnimationComponent || bIsBuilt || !bIsPlaced) {
		return;
	}

	//build
	if (m_currentBuiltAmount < m_pBuildingInfo.m_maxBuildAmount) {
		m_currentBuiltAmount += 0.5f;

		Vec3 currentPos = m_pAnimationComponent->GetTransformMatrix().GetTranslation();
		currentPos.z += 0.12f;
		m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, currentPos));
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
		m_pSelectableComponent->SetIsBuilding(true);
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

SBuildingInfo BuildingComponent::GetBuildingInfos()
{
	return m_pBuildingInfo;
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

	entityItPtr->MoveFirst();
	while (!entityItPtr->IsEnd())
	{
		IEntity* pPlayer = gEnv->pEntitySystem->FindEntityByName(PLAYER_ENTITY_NAME);
		IEntity* other = entityItPtr->Next();
		if (!other || other == m_pEntity || other == pPlayer) {
			continue;
		}

		pd->Begin("BuildingCanBePlaced", true);
		AABB otherAABB;
		other->GetWorldBounds(otherAABB);
		if (aabb.IsIntersectBox(otherAABB) && !other->GetComponent<Cry::DefaultComponents::CEnvironmentProbeComponent>() ) {
			pd->AddAABB(aabb.min, aabb.max, ColorF(1, 0, 0), 0.1f);
			return false;
		}

		//Check Terrain
		f32 minX = aabb.min.x;
		f32 maxX = aabb.max.x;
		f32 minY = aabb.min.y;
		f32 maxY = aabb.max.y;
		f32 minZ = aabb.min.z;

		//X
		f32 xPos = aabb.min.x;
		while (xPos < aabb.max.x) {
			if (crymath::abs(gEnv->p3DEngine->GetTerrainZ(xPos, minY) - minZ) > 0.2f) {
				pd->AddAABB(aabb.min, aabb.max, ColorF(1, 0, 0), 0.1f);
				return false;
			}
			if (crymath::abs(gEnv->p3DEngine->GetTerrainZ(xPos, maxY) - minZ) > 0.2f) {
				pd->AddAABB(aabb.min, aabb.max, ColorF(1, 0, 0), 0.1f);
				return false;
			}
			xPos += 1;
		}

		//Y
		f32 yPos = aabb.min.y;
		while (yPos < aabb.max.y) {
			Vec3 p(minX, yPos, minZ);
			//pd->AddSphere(p, 0.5f, ColorF(0, 0, 1), 0.7f);

			if (crymath::abs(gEnv->p3DEngine->GetTerrainZ(minX, yPos) - minZ) > 0.2f) {
				pd->AddAABB(aabb.min, aabb.max, ColorF(1, 0, 0), 0.1f);
				return false;
			}
			if (crymath::abs(gEnv->p3DEngine->GetTerrainZ(maxX, yPos) - minZ) > 0.2f) {
				pd->AddAABB(aabb.min, aabb.max, ColorF(1, 0, 0), 0.1f);
				return false;
			}
			yPos += 1;
		}
	}
	pd->AddAABB(aabb.min, aabb.max, ColorF(0, 1, 0), 0.1f);
	return true;
}

bool BuildingComponent::IsBuilt()
{
	return bIsBuilt;
}

void BuildingComponent::SetBuilt()
{
	m_currentBuiltAmount = m_pBuildingInfo.m_maxBuildAmount;

	Place(m_pEntity->GetWorldPos());

	bIsPlaced = true;

	//Remove placement decal 
	m_pEntity->RemoveComponent<Cry::DefaultComponents::CDecalComponent>();

	//Physicalize
	SEntityPhysicalizeParams physParams;
	physParams.type = PE_STATIC;
	physParams.mass = 38000.f;
	m_pEntity->Physicalize(physParams);

	AABB aabb;
	m_pEntity->GetWorldBounds(aabb);
	gEnv->pAISystem->GetNavigationSystem()->GetUpdateManager()->WorldChanged(aabb);

	//HealthComponent Initialization
	m_pHealthComponent = m_pEntity->GetOrCreateComponent<HealthComponent>();
	m_pHealthComponent->SetMaxHealth(m_maxHealth);

	//VisibilityComponent Initialization
	m_pVisibilityComponent = m_pEntity->GetOrCreateComponent<VisibilityComponent>();

	Build();
}

void BuildingComponent::SetPathToTrussMesh(string path)
{
	this->m_pathToTrussMesh = path;
}

void BuildingComponent::AddUIItem(IBaseUIItem* item)
{
	this->m_pAllUIItems.append(item);
}

Vec3 BuildingComponent::GetExitPoint()
{
	Vec3 pos = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName(BUILDING_EXIT_POINT_ATTACHMENT_NAME)->GetAttWorldAbsolute().t;
	pos.z = m_pEntity->GetWorldPos().z;
	return pos;
}

void BuildingComponent::SetIsHouse(bool IsHouse)
{
	this->bIsHouse = IsHouse;
}

bool BuildingComponent::IsHouse()
{
	return bIsHouse;
}

bool BuildingComponent::IsWorkplace()
{
	return bIsBuilt && m_pEntity->GetComponent<WorkplaceComponent>();
}

void BuildingComponent::SetMaxHealth(f32 maxHealth)
{
	this->m_maxHealth = maxHealth;
}

f32 BuildingComponent::GetCurrentBuildAmount()
{
	return m_currentBuiltAmount;
}

f32 BuildingComponent::GetMaxBuildAmount()
{
	return m_pBuildingInfo.m_maxBuildAmount;
}

void BuildingComponent::SetImagePath(string imagePath)
{
	this->m_imagePath = imagePath;
}

string BuildingComponent::GetImagePath()
{
	return this->m_imagePath;
}

void BuildingComponent::AddBuilder(IEntity* builderEntity)
{
	this->m_builders.append(builderEntity);
}

void BuildingComponent::RemoveBuilder(IEntity* builderEntity)
{
	DynArray<IEntity*> result;
	for (IEntity* entity : m_builders) {
		if (entity != builderEntity) {
			result.append(entity);
		}
	}

	m_builders.clear();
	m_builders = result;
}

DynArray<IEntity*> BuildingComponent::GetBuilders()
{
	return m_builders;
}

void BuildingComponent::RotateSelectionDecal()
{
	if (!m_pSelectableComponent) {
		return;
	}
	m_pSelectableComponent->RotateSelectionDecal();
}