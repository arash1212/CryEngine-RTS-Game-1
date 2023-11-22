#include "StdAfx.h"
#include "Warehouse1Building.h"
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
#include<UIItems/Items/Resources/UISellOilItem.h>
#include<UIItems/Items/Resources/UIBuyOilItem.h>
#include<UIItems/Items/Resources/UISellWheatItem.h>
#include<UIItems/Items/Resources/UIBuyWheatItem.h>
#include<UIItems/Items/Resources/UISellFlourItem.h>
#include<UIItems/Items/Resources/UIBuyFlourItem.h>
#include<UIItems/Items/Resources/UISellWoodItem.h>
#include<UIItems/Items/Resources/UIBuyWoodItem.h>
#include<UIItems/Items/Resources/UISellBreadItem.h>
#include<UIItems/Items/Resources/UIBuyBreadItem.h>
#include<UIItems/Items/Resources/UISellIronItem.h>
#include<UIItems/Items/Resources/UIBuyIronItem.h>
#include<UIItems/Items/Resources/UISellBulletItem.h>
#include<UIItems/Items/Resources/UIBuyBulletItem.h>

#include <Components/BaseBuilding/Building.h>
#include <Utils/MathUtils.h>
#include <Components/Selectables/Cost.h>

#include <Components/Managers/ResourceManager.h>
#include <Components/Selectables/ResourceStorage.h>

#include <Components/Selectables/Health.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


namespace
{
	static void RegisterWarehouse1BuildingComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Warehouse1BuildingComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterWarehouse1BuildingComponent);
}


void Warehouse1BuildingComponent::Initialize()
{
	//AnimationComponent Initializations
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0, 0, 0)));
	m_pAnimationComponent->SetCharacterFile(WAREHOUSE_BUILDING_1_MODEL_PATH);
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/warehouse1.adb");
	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Idle");
	m_pAnimationComponent->SetAnimationDrivenMotion(false);
	m_pAnimationComponent->LoadFromDisk();
	m_pAnimationComponent->ResetCharacter();

	//BoxComponent Initialization
	m_pBboxComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CBoxPrimitiveComponent>();
	m_pBboxComponent->m_size = Vec3(4.f, 3.9f, 2.3f);
	m_pBboxComponent->m_bReactToCollisions = true;

	//DecalComponent(Placement) Initialization
	m_pDecalComponent = m_pEntity->CreateComponent<Cry::DefaultComponents::CDecalComponent>();
	m_pDecalComponent->SetTransformMatrix(Matrix34::Create(Vec3(4.1f, 5.9f, 3), IDENTITY, Vec3(-0.0f, -1.85f, 0)));
	m_pDecalComponent->SetMaterialFileName(BUILDING_PLACEMENT_GREEN_DECAL_MATERIAL);
	m_pDecalComponent->SetSortPriority(50);
	m_pDecalComponent->SetDepth(10);
	m_pDecalComponent->Spawn();

	//BuildingComponent initialization
	SetPathToTrussMesh(WAREHOUSE_BUILDING_1_TRUSS_MODEL_PATH);
	SBuildingInfo buildingInfo;
	buildingInfo.m_populationProduces = 0;
	SetBuildingInfo(buildingInfo);
	SetMaxHealth(800.f);
	SetImagePath(Warehouse1BuildingComponent::GetDescription().sIcon);
	//UIItems
	AddUIItem(new UIBuyOilItem(m_pEntity));
	AddUIItem(new UISellOilItem(m_pEntity));
	AddUIItem(new UIBuyWheatItem(m_pEntity));
	AddUIItem(new UISellWheatItem(m_pEntity));
	AddUIItem(new UIBuyFlourItem(m_pEntity));
	AddUIItem(new UISellFlourItem(m_pEntity));
	AddUIItem(new UIBuyWoodItem(m_pEntity));
	AddUIItem(new UISellWoodItem(m_pEntity));
	AddUIItem(new UIBuyBreadItem(m_pEntity));
	AddUIItem(new UISellBreadItem(m_pEntity));
	AddUIItem(new UIBuyIronItem(m_pEntity));
	AddUIItem(new UISellIronItem(m_pEntity));
	AddUIItem(new UIBuyBulletItem(m_pEntity));
	AddUIItem(new UISellBulletItem(m_pEntity));

	//ResourceStorageComponent Initialization
	m_pResourceStorageComponent = m_pEntity->GetOrCreateComponent<ResourceStorageComponent>();

	//Update bounding box
	AABB aabb;
	m_pEntity->GetLocalBounds(aabb);
	Vec3 min = Vec3(aabb.min.x - 4.0f, aabb.min.y - 4, aabb.min.z - 0.8f);
	Vec3 max = Vec3(aabb.max.x + 4.1f, aabb.max.y + 4.f, aabb.max.z);;
	AABB newAABB = AABB(min, max);
	m_pEntity->SetLocalBounds(newAABB, true);

	//OwnerComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetComponent<OwnerInfoComponent>();

	//CostComponent Initializations
	m_pCostComponent = m_pEntity->GetOrCreateComponent<CostComponent>();
	m_pCostComponent->SetCost(Warehouse1BuildingComponent::GetDescription().price);

	//ActionManager Initializations
	m_pActionManagerComponent = m_pEntity->GetOrCreateComponent<ActionManagerComponent>();
	m_pActionManagerComponent->SetIsBuilding(true);

	//SkinAttachment Initialization
	m_pSkinAttachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByIndex(0);

	//Materials Initializations
	m_pDefaultMaterial = m_pSkinAttachment->GetIAttachmentObject()->GetBaseMaterial();

}


Cry::Entity::EventFlags Warehouse1BuildingComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void Warehouse1BuildingComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];

		UpdateMaterial();
		RotateSelectionDecal();

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

SDescription Warehouse1BuildingComponent::GetDescription()
{
	SResourceInfo price;
	price.m_moneyAmount = 40;
	price.m_populationAmount = 0;
	price.m_ironAmount = 100;
	price.m_woodAmount = 100;

	SDescription m_pDescription;
	m_pDescription.sName = "Warehouse 1";
	m_pDescription.sDescription = "Warehouse 1 Building.";
	m_pDescription.sBuyDescription = "Build Warehouse 1 Building.";
	m_pDescription.price = price;
	m_pDescription.sIcon = "warehouse_1_Icon.png";

	return m_pDescription;
}