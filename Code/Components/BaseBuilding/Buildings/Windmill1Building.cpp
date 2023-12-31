#include "StdAfx.h"
#include "Windmill1Building.h"
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

#include <Components/Selectables/Worker.h>
#include <Components/Selectables/Workplace.h>
#include <Components/Selectables/ResourceCollector.h>
#include <Components/Selectables/ResourceStorage.h>

#include <Components/BaseBuilding/Building.h>
#include <Components/Selectables/Cost.h>

#include <Utils/MathUtils.h>
#include <Utils/EntityUtils.h>

#include <Components/Managers/ResourceManager.h>
#include <Components/Selectables/ResourceStorage.h>
#include <Resources/IResource.h>

#include <Components/Selectables/Health.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


namespace
{
	static void RegisterWindmill1BuildingComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Windmill1BuildingComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterWindmill1BuildingComponent);
}


void Windmill1BuildingComponent::Initialize()
{
	//AnimationComponent Initializations
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0, 0, 0)));
	m_pAnimationComponent->SetCharacterFile(WINDMILL_BUILDING_1_MODEL_PATH);
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/windmill.adb");
	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Idle");
	m_pAnimationComponent->SetAnimationDrivenMotion(false);
	m_pAnimationComponent->LoadFromDisk();
	m_pAnimationComponent->ResetCharacter();

	//BoxComponent Initialization
	m_pBboxComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CBoxPrimitiveComponent>();
	m_pBboxComponent->m_size = Vec3(1.7f, 1.6f, 4.5f);
	m_pBboxComponent->m_bReactToCollisions = true;

	//DecalComponent(Placement) Initialization
	m_pDecalComponent = m_pEntity->CreateComponent<Cry::DefaultComponents::CDecalComponent>();
	m_pDecalComponent->SetTransformMatrix(Matrix34::Create(Vec3(6.1f, 4.6f, 3), IDENTITY, Vec3(-0.05f, -0.03f, 0)));
	m_pDecalComponent->SetMaterialFileName(BUILDING_PLACEMENT_GREEN_DECAL_MATERIAL);
	m_pDecalComponent->SetSortPriority(50);
	m_pDecalComponent->SetDepth(10);
	m_pDecalComponent->Spawn();

	//BuildingComponent initialization
	SetPathToTrussMesh(WINDMILL_BUILDING_1_TRUSS_MODEL_PATH);
	SetImagePath(Windmill1BuildingComponent::GetDescription().sIcon);

	SBuildingInfo buildingInfo;
	buildingInfo.m_populationProduces = 0;
	SetBuildingInfo(buildingInfo);
	SetMaxHealth(600.f);
	//UIItems


	//Update bounding box
	AABB aabb;
	m_pEntity->GetLocalBounds(aabb);
	Vec3 min = Vec3(aabb.min.x - 6.f, aabb.min.y - 2, aabb.min.z - 2.72f);
	Vec3 max = Vec3(aabb.max.x + 6.f, aabb.max.y + 6, aabb.max.z);
	AABB newAABB = AABB(min, max);
	m_pEntity->SetLocalBounds(newAABB, true);

	//OwnerComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetComponent<OwnerInfoComponent>();

	//CostComponent Initializations
	m_pCostComponent = m_pEntity->GetOrCreateComponent<CostComponent>();
	m_pCostComponent->SetCost(Windmill1BuildingComponent::GetDescription().price);

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
}


Cry::Entity::EventFlags Windmill1BuildingComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void Windmill1BuildingComponent::ProcessEvent(const SEntityEvent& event)
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

void Windmill1BuildingComponent::UpdateAssignedWorkers()
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
	int32 WheatRequestAmount = 30;
	int32 FlourProducedAmount = 10;
	Vec3 workPosition = GetExitPoint();

	//**********************************Move to Warehouse and pickup some wheat And Transer to mill
	if (!bIsCollectedWheatAndTransferedWheatToMill) {
		if (pWorkerComponent->PickResourceFromWarehouseAndTransferToPosition(EResourceType::WHEAT, WheatRequestAmount, workPosition)) {
			bIsCollectedWheatAndTransferedWheatToMill = true;
		}
	}

	//**********************************Produce Flour
	if (bIsCollectedWheatAndTransferedWheatToMill && !bIsProducedFlour) {
		if (pWorkerComponent->WaitAndPickResources(productionWaitAmount, workPosition, m_pEntity->GetWorldPos(), EResourceType::FLOUR, FlourProducedAmount)) {
			bIsProducedFlour = true;
		}
	}

	//**********************************Transfer Flour to warehouse
	if (bIsCollectedWheatAndTransferedWheatToMill && bIsProducedFlour) {
		if (pWorkerComponent->TransferResourcesToWarehouse(EResourceType::FLOUR, FlourProducedAmount)) {
			bIsCollectedWheatAndTransferedWheatToMill = false;
			bIsProducedFlour = false;
			pWorkerComponent->SetHasEnteredWorkplace(false);
		}
	}
}

SDescription Windmill1BuildingComponent::GetDescription()
{
	SResourceInfo price;
	price.m_moneyAmount = 150;
	price.m_oilAmount = 200;
	price.m_woodAmount = 300;

	SDescription m_pDescription;
	m_pDescription.sName = "Windmill 1";
	m_pDescription.sDescription = "Windmill 1 Building.";
	m_pDescription.sBuyDescription = "Build Windmill 1 Building.";
	m_pDescription.price = price;
	m_pDescription.sIcon = "windmill_1_icon.png";

	return m_pDescription;
}