#include "StdAfx.h"
#include "Light1Building.h"
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

#include <Components/BaseBuilding/Building.h>
#include <Utils/MathUtils.h>
#include <Components/Selectables/Cost.h>

#include <Components/Managers/ResourceManager.h>

#include <Components/Selectables/Health.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


namespace
{
	static void RegisterLight1BuildingComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Light1BuildingComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterLight1BuildingComponent);
}


void Light1BuildingComponent::Initialize()
{
	//AnimationComponent Initializations
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0, 0, 0)));
	m_pAnimationComponent->SetCharacterFile(LIGHT_BUILDING_1_MODEL_PATH);
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/light1.adb");
	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Idle");
	m_pAnimationComponent->SetAnimationDrivenMotion(false);
	m_pAnimationComponent->LoadFromDisk();
	m_pAnimationComponent->ResetCharacter();

	//BoxComponent Initialization
	m_pBboxComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CBoxPrimitiveComponent>();
	m_pBboxComponent->m_size = Vec3(0.2f, 0.2f, 3.0f);
	m_pBboxComponent->m_bReactToCollisions = true;

	//DecalComponent(Placement) Initialization
	m_pDecalComponent = m_pEntity->CreateComponent<Cry::DefaultComponents::CDecalComponent>();
	m_pDecalComponent->SetTransformMatrix(Matrix34::Create(Vec3(0.45f, 1.15f, 2), IDENTITY, Vec3(0.0f, -0.63f, 0)));
	m_pDecalComponent->SetMaterialFileName(BUILDING_PLACEMENT_GREEN_DECAL_MATERIAL);
	m_pDecalComponent->SetSortPriority(50);
	m_pDecalComponent->SetDepth(10);
	m_pDecalComponent->Spawn();

	//BuildingComponent initialization
	SetPathToTrussMesh(LIGHT_BUILDING_1_TRUSS_MODEL_PATH);
	SetIsHouse(false);
	SBuildingInfo buildingInfo;
	buildingInfo.m_populationProduces = 20;
	SetBuildingInfo(buildingInfo);
	SetMaxHealth(100.f);
	SetImagePath(Light1BuildingComponent::GetDescription().sIcon);
	//UIItems

	//Update bounding box
	AABB aabb;
	m_pEntity->GetLocalBounds(aabb);
	Vec3 min = Vec3(aabb.min.x - 0.4f, aabb.min.y - 0.5f, aabb.min.z - 0.016f);
	Vec3 max = Vec3(aabb.max.x + 0.4f, aabb.max.y + 0.5f, aabb.max.z);
	AABB newAABB = AABB(min, max);
	m_pEntity->SetLocalBounds(newAABB, true);

	//CostComponent Initialization
	m_pCostComponent = m_pEntity->GetOrCreateComponent<CostComponent>();
	m_pCostComponent->SetCost(Light1BuildingComponent::GetDescription().price);

	//ActionManager Initializations
	m_pActionManagerComponent = m_pEntity->GetOrCreateComponent<ActionManagerComponent>();
	m_pActionManagerComponent->SetIsBuilding(true);

	//SkinAttachment Initialization
	m_pSkinAttachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByIndex(0);

	//Materials Initializations
	m_pDefaultMaterial = m_pSkinAttachment->GetIAttachmentObject()->GetBaseMaterial();

	//LightPosAttachment Initialization
	m_pLightPosAttachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("lightPos");

	//PointLightComponent Initialization
	m_pPointLightComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CPointLightComponent>();
	m_pPointLightComponent->SetTransformMatrix(Matrix34::Create(Vec3(2), IDENTITY, Vec3(0, -2, 5)));
	m_pPointLightComponent->GetColorParameters().m_diffuseMultiplier = 2.f;
	m_pPointLightComponent->GetColorParameters().m_color = ColorF(1, 1, 1.f);
	m_pPointLightComponent->GetOptions().m_attenuationBulbSize = 250.f;
	m_pPointLightComponent->GetShadowParameters().m_castShadowSpec = Cry::DefaultComponents::EMiniumSystemSpec::High;
	m_pPointLightComponent->Enable(true);
}


Cry::Entity::EventFlags Light1BuildingComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void Light1BuildingComponent::ProcessEvent(const SEntityEvent& event)
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

SDescription Light1BuildingComponent::GetDescription()
{
	SResourceInfo price;
	price.m_moneyAmount = 100;
	price.m_populationAmount = 0;
	price.m_ironAmount = 300;

	SDescription m_pDescription;
	m_pDescription.sName = "Light 1";
	m_pDescription.sDescription = "Light 1 Building.";
	m_pDescription.sBuyDescription = "Build Light 1 Building.";
	m_pDescription.price = price;
	m_pDescription.sIcon = "light_1_icon.png";

	return m_pDescription;
}