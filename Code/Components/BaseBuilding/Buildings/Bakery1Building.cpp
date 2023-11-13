#include "StdAfx.h"
#include "Bakery1Building.h"
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

#include <Components/BaseBuilding/Building.h>
#include <Utils/MathUtils.h>
#include <Components/Selectables/Cost.h>

#include <Components/Managers/ResourceManager.h>

#include <Components/Selectables/Worker.h>
#include <Components/Selectables/Workplace.h>
#include <Components/Selectables/ResourceCollector.h>
#include <Components/Selectables/ResourceStorage.h>

#include <Utils/EntityUtils.h>

#include <Components/Selectables/Health.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


namespace
{
	static void RegisterBakery1BuildingComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Bakery1BuildingComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterBakery1BuildingComponent);
}


void Bakery1BuildingComponent::Initialize()
{
	//AnimationComponent Initializations
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0, 0, 0)));
	m_pAnimationComponent->SetCharacterFile(BAKERY_BUILDING_1_MODEL_PATH);
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/bakery1.adb");
	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Idle");
	m_pAnimationComponent->SetAnimationDrivenMotion(false);
	m_pAnimationComponent->LoadFromDisk();
	m_pAnimationComponent->ResetCharacter();

	//BoxComponent Initialization
	m_pBboxComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CBoxPrimitiveComponent>();
	m_pBboxComponent->m_size = Vec3(3.2f, 2.0f, 3.4f);
	m_pBboxComponent->m_bReactToCollisions = true;

	//DecalComponent(Placement) Initialization
	m_pDecalComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CDecalComponent>();
	m_pDecalComponent->SetTransformMatrix(Matrix34::Create(Vec3(4.f, 5.7f, 3), IDENTITY, Vec3(0.3f, -2.4, 0)));
	m_pDecalComponent->SetMaterialFileName(BUILDING_PLACEMENT_GREEN_DECAL_MATERIAL);
	m_pDecalComponent->SetSortPriority(50);
	m_pDecalComponent->SetDepth(10);
	m_pDecalComponent->Spawn();

	//BuildingComponent initialization
	SetPathToTrussMesh(BAKERY_BUILDING_1_TRUSS_MODEL_PATH);
	SBuildingInfo buildingInfo;
	buildingInfo.m_populationProduces = 0;
	SetBuildingInfo(buildingInfo);
	SetImagePath(Bakery1BuildingComponent::GetDescription().sIcon);
	//UIItems
	SetMaxHealth(500.f);

	//Update bounding box
	AABB aabb;
	m_pEntity->GetLocalBounds(aabb);
	Vec3 min = Vec3(aabb.min.x - 3.6f, aabb.min.y - 10.f, aabb.min.z);
	Vec3 max = Vec3(aabb.max.x + 3.1f, aabb.max.y + -0.9f, aabb.max.z);
	AABB newAABB = AABB(min, max);
	m_pEntity->SetLocalBounds(newAABB, true);

	//CostComponent Initializations
	m_pCostComponent = m_pEntity->GetOrCreateComponent<CostComponent>();
	m_pCostComponent->SetCost(Bakery1BuildingComponent::GetDescription().price);

	//ActionManager Initializations
	m_pActionManagerComponent = m_pEntity->GetOrCreateComponent<ActionManagerComponent>();
	m_pActionManagerComponent->SetIsBuilding(true);

	//SkinAttachment Initialization
	m_pSkinAttachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByIndex(0);

	//Materials Initializations
	m_pDefaultMaterial = m_pSkinAttachment->GetIAttachmentObject()->GetBaseMaterial();

	//WorkplaceComponent  Initializations
	m_pWorkplaceComponent = m_pEntity->GetOrCreateComponent<WorkplaceComponent>();
	m_pWorkplaceComponent->SetMaxWorkersCount(1);

	//WorkPositionAttachment
	m_pWorkPositionAttachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("workPosition1");

	//ParticleComponent Initialization
	m_pParticleComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CParticleComponent>();
	m_pParticleComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(2.5f, -1.2f, 2)));
	m_pParticleComponent->SetEffectName("Objects/effects/smoke/smoke_particle_1.pfx");
	m_pParticleComponent->Activate(false);
}


Cry::Entity::EventFlags Bakery1BuildingComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void Bakery1BuildingComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];

		UpdateMaterial();
		RotateSelectionDecal();
		UpdateAssignedWorkers();

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

void Bakery1BuildingComponent::UpdateAssignedWorkers()
{
	if (!IsBuilt()) {
		return;
	}
	IEntity* pWorker = m_pWorkplaceComponent->GetWorkers()[0];
	if (!pWorker || pWorker->IsGarbage()) {
		return;
	}
	WorkerComponent* pWorkerComponent = pWorker->GetComponent<WorkerComponent>();
	if (!pWorkerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "Bakery1BuildingComponent:(UpdateCurrentMoveToAttachment) pWorkerComponent is null");
		return;
	}
	if (!pWorkerComponent->HasEnteredWorkplace()) {
		return;
	}

	int32 productionWaitAmount = 5;
	int32 FlourRequestAmount = 20;
	int32 BreadProducedAmount = 10;
	Vec3 workPosition = m_pWorkPositionAttachment->GetAttWorldAbsolute().t;

	//**********************************Move to Warehouse and pickup some Flour And Transfer To Bakery
	if (!bIsCollectedAndTransferedFlour) {
		if (pWorkerComponent->PickResourceFromWarehouseAndTransferToPosition(EResourceType::FLOUR, FlourRequestAmount, workPosition)) {
			bIsCollectedAndTransferedFlour = true;
			m_pParticleComponent->Activate(true);
		}
	}

	//**********************************Produce Bread
	if (bIsCollectedAndTransferedFlour && !bIsProducedBread) {
		if (pWorkerComponent->WaitAndPickResources(productionWaitAmount, workPosition, workPosition, EResourceType::BREAD, BreadProducedAmount)) {
			bIsProducedBread = true;
			m_pParticleComponent->Activate(false);
		}
	}

	//**********************************Transfer Bread to warehouse
	if (bIsCollectedAndTransferedFlour && bIsProducedBread) {
		if (pWorkerComponent->TransferResourcesToWarehouse(EResourceType::BREAD, BreadProducedAmount)) {
			bIsCollectedAndTransferedFlour = false;
			bIsProducedBread = false;
			pWorkerComponent->SetHasEnteredWorkplace(false);
		}
	}
}

void Bakery1BuildingComponent::ResetWorkersJob()
{
}

SDescription Bakery1BuildingComponent::GetDescription()
{
	SResourceInfo price;
	price.m_moneyAmount = 300;
	price.m_oilAmount = 300;
	price.m_woodAmount = 700;
	price.m_ironAmount = 500;
	price.m_bulletAmount = 0;

	SDescription m_pDescription;
	m_pDescription.sName = "Bakery 1";
	m_pDescription.sDescription = "Bakery 1 Building.";
	m_pDescription.sBuyDescription = "Build Bakery 1 Building.";
	m_pDescription.price = price;
	m_pDescription.sIcon = "bakery_1_icon.png";

	return m_pDescription;
}