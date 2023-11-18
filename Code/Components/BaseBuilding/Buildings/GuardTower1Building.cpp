#include "StdAfx.h"
#include "GuardTower1Building.h"
#include "GamePlugin.h"

#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Controller/AIController.h>
#include <Components/Weapons/BaseWeapon.h>
#include <Components/Selectables/UnitAnimation.h>
#include <Components/Managers/ActionManager.h>

#include <Components/Selectables/Selectable.h>
#include <UIItems/IBaseUIItem.h>
#include <UIItems/Items/UICancelItem.h>
#include <UIItems/Items/UIChangeStanceItem.h>
#include <UIItems/Items/Buildings/UIHQ1BuildItem.h>
#include<UIItems/Items/Buildings/TrainUnits/UITrainEngineer1Item.h>
#include<UIItems/Items/Buildings/TrainUnits/UITrainSoldier1Item.h>
#include <Components/Selectables/GuardPost.h>

#include <Components/BaseBuilding/Buildings/Wall1Building.h>

#include <Utils/MathUtils.h>
#include <Utils/EntityUtils.h>
#include <Components/Selectables/Cost.h>

#include <Components/Managers/ResourceManager.h>

#include <Components/Selectables/Health.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


namespace
{
	static void RegisterGaurdTower1BuildingComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(GaurdTower1BuildingComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterGaurdTower1BuildingComponent);
}


void GaurdTower1BuildingComponent::Initialize()
{
	//AnimationComponent Initializations
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0, 0, 0)));
	m_pAnimationComponent->SetCharacterFile(GUARD_TOWER_BUILDING_1_MODEL_PATH);
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/guardtower1.adb");
	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Idle");
	m_pAnimationComponent->SetAnimationDrivenMotion(false);
	m_pAnimationComponent->LoadFromDisk();
	m_pAnimationComponent->ResetCharacter();

	//BoxComponent Initialization
	m_pBboxComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CBoxPrimitiveComponent>();
	m_pBboxComponent->m_size = Vec3(1.8f, 1.9f, 4.1f);
	m_pBboxComponent->m_bReactToCollisions = true;

	//DecalComponent(Placement) Initialization
	m_pDecalComponent = m_pEntity->CreateComponent<Cry::DefaultComponents::CDecalComponent>();
	m_pDecalComponent->SetTransformMatrix(Matrix34::Create(Vec3(2.3f, 2.3f, 3), IDENTITY, Vec3(-0.0f, -0.0, 0)));
	m_pDecalComponent->SetMaterialFileName(BUILDING_PLACEMENT_GREEN_DECAL_MATERIAL);
	m_pDecalComponent->SetSortPriority(50);
	m_pDecalComponent->SetDepth(10);
	m_pDecalComponent->Spawn();

	//BuildingComponent initialization
	SetPathToTrussMesh(GUARD_TOWER_BUILDING_1_TRUSS_MODEL_PATH);
	SBuildingInfo buildingInfo;
	buildingInfo.m_populationProduces = 0;
	SetBuildingInfo(buildingInfo);
	SetImagePath(GaurdTower1BuildingComponent::GetDescription().sIcon);
	//m_pBuildingComponent->SetMaxHealth(700.f);
	//UIItems

	//Update bounding box
	AABB aabb;
	m_pEntity->GetLocalBounds(aabb);
	Vec3 min = Vec3(aabb.min.x - 2.1f, aabb.min.y - 2.1f, aabb.min.z);
	Vec3 max = Vec3(aabb.max.x + 2.1f, aabb.max.y + 2.1f, aabb.max.z);
	AABB newAABB = AABB(min, max);
	m_pEntity->SetLocalBounds(newAABB, true);

	//OwnerComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetComponent<OwnerInfoComponent>();

	//CostComponent Initializations
	m_pCostComponent = m_pEntity->GetOrCreateComponent<CostComponent>();
	m_pCostComponent->SetCost(GaurdTower1BuildingComponent::GetDescription().price);

	//ActionManager Initializations
	m_pActionManagerComponent = m_pEntity->GetOrCreateComponent<ActionManagerComponent>();
	m_pActionManagerComponent->SetIsBuilding(true);

	//SkinAttachment Initialization
	m_pSkinAttachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByIndex(0);

	//Materials Initializations
	m_pDefaultMaterial = m_pSkinAttachment->GetIAttachmentObject()->GetBaseMaterial();

	//GuardPostComponent Initialization
	m_pGuardPostComponent = m_pEntity->GetOrCreateComponent<GuardPostComponent>();
	DynArray<IAttachment*> guardPointAttachments;
	guardPointAttachments.append(m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("guardPoint1"));
	guardPointAttachments.append(m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("guardPoint2"));
	guardPointAttachments.append(m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("guardPoint3"));
	guardPointAttachments.append(m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("guardPoint4"));
	m_pGuardPostComponent->SetGuardPointAttachments(guardPointAttachments);
}


Cry::Entity::EventFlags GaurdTower1BuildingComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Remove |
		Cry::Entity::EEvent::Reset;
}

void GaurdTower1BuildingComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];

		UpdateMaterial();
		RotateSelectionDecal();
		//
		CheckForOtherTowers();

	}break;
	case Cry::Entity::EEvent::Remove: {
		for (IEntity* pWallEntity : m_pWalls) {
			EntityUtils::RemoveEntity(pWallEntity);
		}

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

void GaurdTower1BuildingComponent::CheckForOtherTowers()
{
	if (IsPlaced()) {
		return;
	}
	if (!m_pOwnerInfoComponent) {
		return;
	}
	IEntity* pOwner = m_pOwnerInfoComponent->GetOwner();
	if (!pOwner) {
		return;
	}

	for (IEntity* entity : pOwner->GetComponent<ResourceManagerComponent>()->GetOwnedEntities()) {
		if (entity == m_pEntity) {
			continue;
		}
		if (!entity->GetComponent< GaurdTower1BuildingComponent>()) {
			continue;
		}

		IPersistantDebug* pd = gEnv->pGameFramework->GetIPersistantDebug();
		pd->Begin("towertesting", true);

		for (IEntity* entity : m_pWalls) {
			if (!entity || entity->IsGarbage()) {
				continue;
			}
			pd->AddSphere(entity->GetWorldPos(), 0.7f, ColorF(0, 0, 1), 0.1f);
		}

		bool bShouldClearWalls = true;
		if (m_lastCheckPos != m_pEntity->GetWorldPos()) {
			for (IEntity* pWallEntity : m_pWalls) {
				EntityUtils::RemoveEntity(pWallEntity);
			}
			m_pWalls.clear();
			if (entity->GetWorldPos().x == m_pEntity->GetWorldPos().x || entity->GetWorldPos().y == m_pEntity->GetWorldPos().y) {
				m_lastCheckPos = m_pEntity->GetWorldPos();

				Vec3 pos1 = m_pEntity->GetWorldPos();
				pos1.z += 2;
				Vec3 pos2 = entity->GetWorldPos();
				pos2.z += 2;
				pd->AddLine(pos1, pos2, ColorF(0, 1, 0), 0.1f);

				f32 distance = pos1.GetDistance(pos2);
				Vec3 dir = (pos2 - pos1).normalized();
				f32 width = 2.7f;
				int32 count = (int32)(distance / width);
				for (int32 i = 0; i < count - 1; i++) {
					//Vec3 wallPos = pos1 + (pos2 - pos1) / 2;
					Vec3 pos = m_pEntity->GetWorldPos();
					f32 diff = 1.f;
					//x
					if (pos2.x > pos.x) {
						pos.x -= diff;
					}
					else if (pos2.x < pos.x) {
						pos.x += diff;
					}

					//y
					if (pos2.y > pos.y) {
						pos.y -= diff;
					}
					else if (pos2.y < pos.y) {
						pos.y += diff;
					}

					Vec3 wallPos = pos + dir * (distance - (width * (i + 1)));
					IEntity* pWallEntity = EntityUtils::SpawnEntity(wallPos, IDENTITY, pOwner);
					pWallEntity->SetRotation(Quat::CreateRotationVDir(dir));

					pWallEntity->GetOrCreateComponent<Wall1BuildingComponent>();
					m_pWalls.append(pWallEntity);

					bShouldClearWalls = false;
				}
				//return;
			}
			else {
				if (bShouldClearWalls) {
					for (IEntity* pWallEntity : m_pWalls) {
						EntityUtils::RemoveEntity(pWallEntity);
					}
					m_pWalls.clear();
				}
			}
			//}
		}
	}
}


void GaurdTower1BuildingComponent::Place(Vec3 at)
{
	BuildingComponent::Place(at);

	for (IEntity* pWallEntity : m_pWalls) {
		BuildingComponent* pBuildingComponent = pWallEntity->GetComponent<BuildingComponent>();
		if (pBuildingComponent->CanBePlaced()) {
			pBuildingComponent->Place(pWallEntity->GetWorldPos());
		}
		else {
			EntityUtils::RemoveEntity(pWallEntity);
		}
	}
}

SDescription GaurdTower1BuildingComponent::GetDescription()
{
	SResourceInfo price;
	price.m_moneyAmount = 150;
	price.m_oilAmount = 0;
	price.m_populationAmount = 0;
	price.m_ironAmount = 200;
	price.m_woodAmount = 100;


	SDescription m_pDescription;
	m_pDescription.sName = "GuardTower 1";
	m_pDescription.sDescription = "GuardTower 1 Building.";
	m_pDescription.sBuyDescription = "guard_tower_1_icon.png";
	m_pDescription.price = price;
	m_pDescription.sIcon = "guard_tower_1_icon.png";

	return m_pDescription;
}