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
	m_pBuildingComponent = m_pEntity->GetOrCreateComponent<BuildingComponent>();
	m_pBuildingComponent->SetPathToTrussMesh(WINDMILL_BUILDING_1_TRUSS_MODEL_PATH);
	SBuildingInfo buildingInfo;
	buildingInfo.m_populationProduces = 0;
	m_pBuildingComponent->SetBuildingInfo(buildingInfo);
	m_pBuildingComponent->SetMaxHealth(600.f);
	//UIItems


	//Update bounding box
	AABB aabb;
	m_pEntity->GetLocalBounds(aabb);
	Vec3 min = Vec3(aabb.min.x - 6.f, aabb.min.y - 2, aabb.min.z - 2.72f);
	Vec3 max = Vec3(aabb.max.x + 6.f, aabb.max.y + 6, aabb.max.z);
	AABB newAABB = AABB(min, max);
	m_pEntity->SetLocalBounds(newAABB, true);

	//CostComponent Initializations
	m_pCostComponent = m_pEntity->GetOrCreateComponent<CostComponent>();
	m_pCostComponent->SetCost(Windmill1BuildingComponent::GetCost());

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
	if (!m_pBuildingComponent) {
		return;
	}
	if (!m_pBuildingComponent->IsBuilt()) {
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
	Vec3 workPosition = m_pBuildingComponent->GetExitPoint();

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

SResourceInfo Windmill1BuildingComponent::GetCost()
{
	SResourceInfo cost;
	cost.m_moneyAmount = 350;
	cost.m_oilAmount = 200;
	//cost.m_populationAmount = 2;
	cost.m_woodAmount = 700;
	return cost;
}