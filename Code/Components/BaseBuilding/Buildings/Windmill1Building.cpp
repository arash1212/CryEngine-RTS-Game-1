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
	m_pDecalComponent->SetTransformMatrix(Matrix34::Create(Vec3(6.1f, 5.9f, 3), IDENTITY, Vec3(-0.1f, 0.17f, 0)));
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
	//UIItems


	//Update bounding box
	AABB aabb;
	m_pEntity->GetLocalBounds(aabb);
	Vec3 min = Vec3(aabb.min.x - 6.f, aabb.min.y - 2, aabb.min.z - 2);
	Vec3 max = Vec3(aabb.max.x + 6.f, aabb.max.y + 6, aabb.max.z);;
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
		f32 DeltaTime = event.fParam[0];

		UpdateAssignedWorkers();

		//Timers
		if (m_workTimePassed < m_timeBetweenWorks) {
			m_workTimePassed += 0.5f * DeltaTime;
		}

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
	if (m_pWorkplaceComponent->GetCurrentWorkersCount() <= 0) {
		return;
	}
	OwnerInfoComponent* ownerInfo = m_pEntity->GetComponent<OwnerInfoComponent>();
	if (!ownerInfo) {
		return;
	}
	IEntity* pOwner = ownerInfo->GetOwner();
	if (!pOwner) {
		return;
	}
	ResourceManagerComponent* pResourceManager = pOwner->GetComponent<ResourceManagerComponent>();
	if (!pResourceManager) {
		return;
	}
	if (!m_pWorkplaceComponent->GetWorkers()[0]->GetComponent<WorkerComponent>()->HasEnteredWorkplace()) {
		return;
	}
	IEntity* pWorker = m_pWorkplaceComponent->GetWorkers()[0];
	if (!pWorker) {
		return;
	}
	AIControllerComponent* pAIController = pWorker->GetComponent<AIControllerComponent>();
	if (!pAIController) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "Windmill1BuildingComponent:(UpdateCurrentMoveToAttachment) pAIController is null");
		return;
	}
	ResourceCollectorComponent* pResourceCollectorComponent = pWorker->GetComponent<ResourceCollectorComponent>();
	if (!pResourceCollectorComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "Windmill1BuildingComponent:(UpdateCurrentMoveToAttachment) pResourceCollectorComponent is null");
		return;
	}
	WorkerComponent* pWorkerComponent = pWorker->GetComponent<WorkerComponent>();
	if (!pWorkerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "Windmill1BuildingComponent:(UpdateCurrentMoveToAttachment) pWorkerComponent is null");
		return;
	}

	int32 WheatRequestAmount = 30;
	int32 FlourProducedAmount = 10;

	//**********************************Move to Warehouse and pickup some wheat
	if (!bIsCollectedWheat) {
		SResourceInfo pResourceRequest;
		pResourceRequest.m_wheatAmount = WheatRequestAmount;
		if (!pResourceManager->CheckIfResourcesAvailable(pResourceRequest)) {
			return;
		}
		if (!m_pWarehouseEntity) {
			m_pWarehouseEntity = FindClosestWarehouse();
			return;
		}

		Vec3 warehouseExitPoint = m_pWarehouseEntity->GetComponent<BuildingComponent>()->GetExitPoint();
		//Move closer to warehouse if it's not close
		f32 distanceToWareHouse = EntityUtils::GetDistance(m_pWorkplaceComponent->GetWorkers()[0]->GetWorldPos(), warehouseExitPoint, nullptr);
		if (m_pWarehouseEntity && distanceToWareHouse > 1) {
			pAIController->MoveTo(warehouseExitPoint, false);
			pAIController->LookAtWalkDirection();
		}
		//Pickup Wheat from Warehouse
		else {
			pAIController->StopMoving();
			pAIController->LookAt(m_pWarehouseEntity->GetWorldPos());
			pResourceManager->RequsetResources(pResourceRequest);
			pResourceCollectorComponent->AddResource(WheatRequestAmount);
			pResourceCollectorComponent->SetCurrentResourceType(EResourceType::WHEAT);

			bIsCollectedWheat = true;
		}
	}

	//**********************************Transfer Wheat to mill
	if (bIsCollectedWheat && !bIsTransferedWheatToMill) {
		Vec3 millExitPoint = m_pBuildingComponent->GetExitPoint();
		//Move closer to warehouse if it's not close
		f32 distanceToMill = EntityUtils::GetDistance(m_pWorkplaceComponent->GetWorkers()[0]->GetWorldPos(), millExitPoint, nullptr);
		if (m_pWarehouseEntity && distanceToMill > 1) {
			pAIController->MoveTo(millExitPoint, false);
			pAIController->LookAtWalkDirection();
			m_workTimePassed = 0;
		}
		//Pickup Resources from Warehouse
		else {
			pAIController->StopMoving();
			pAIController->LookAt(m_pEntity->GetWorldPos());
			pResourceCollectorComponent->EmptyResources();

			if (m_workTimePassed >= m_timeBetweenWorks) {
				bIsTransferedWheatToMill = true;

				pResourceCollectorComponent->AddResource(FlourProducedAmount);
				pResourceCollectorComponent->SetCurrentResourceType(EResourceType::FLOUR);
			}
		}
	}

	//**********************************Produce Flour
	if (bIsCollectedWheat && bIsTransferedWheatToMill) {
		if (!m_pWarehouseEntity) {
			m_pWarehouseEntity = FindClosestWarehouse();
			return;
		}

		Vec3 warehouseExitPoint = m_pWarehouseEntity->GetComponent<BuildingComponent>()->GetExitPoint();
		//Move closer to warehouse if it's not close
		f32 distanceToWareHouse = EntityUtils::GetDistance(m_pWorkplaceComponent->GetWorkers()[0]->GetWorldPos(), warehouseExitPoint, nullptr);
		if (m_pWarehouseEntity && distanceToWareHouse > 1) {
			pAIController->MoveTo(warehouseExitPoint, false);
			pAIController->LookAtWalkDirection();
		}
		//Transer Flour to Warehouse
		else {
			pAIController->StopMoving();
			pAIController->LookAt(m_pWarehouseEntity->GetWorldPos());
			pResourceManager->AddResource(EResourceType::FLOUR, FlourProducedAmount);
			pResourceCollectorComponent->EmptyResources();

			bIsCollectedWheat = false;
			bIsTransferedWheatToMill = false;
			pWorkerComponent->SetHasEnteredWorkplace(false);
		}
	}
}

IEntity* Windmill1BuildingComponent::FindClosestWarehouse()
{
	IEntityItPtr entityItPtr = gEnv->pEntitySystem->GetEntityIterator();
	entityItPtr->MoveFirst();
	while (!entityItPtr->IsEnd()) {
		IEntity* entity = entityItPtr->Next();
		if (entity) {
			/*
			BuildingComponent* pBuildingComponent = entity->GetComponent<BuildingComponent>();
			if (!pBuildingComponent) {
				CryLog("Building component not found");
				return nullptr;
			}
			if (!pBuildingComponent->IsBuilt()) {
				return nullptr;
			}
			*/
			ResourceStorageComponent* resourceStorage = entity->GetComponent<ResourceStorageComponent>();
			if (resourceStorage) {
				return entity;
			}
		}
	}
	return nullptr;
}

SResourceInfo Windmill1BuildingComponent::GetCost()
{
	SResourceInfo cost;
	cost.m_moneyAmount = 120;
	cost.m_oilAmount = 100;
	cost.m_populationAmount = 3;
	return cost;
}