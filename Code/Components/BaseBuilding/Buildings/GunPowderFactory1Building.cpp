#include "StdAfx.h"
#include "GunPowderFactory1Building.h"
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
	static void RegisterGunPowderFactory1BuildingComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(GunPowderFactory1BuildingComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterGunPowderFactory1BuildingComponent);
}


void GunPowderFactory1BuildingComponent::Initialize()
{
	//AnimationComponent Initializations
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0, 0, 0)));
	m_pAnimationComponent->SetCharacterFile(GUNPOWDER_FACTORY_BUILDING_1_MODEL_PATH);
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/barracks1.adb");
	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Idle");
	m_pAnimationComponent->SetAnimationDrivenMotion(false);
	m_pAnimationComponent->LoadFromDisk();
	m_pAnimationComponent->ResetCharacter();

	//BoxComponent Initialization
	m_pBboxComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CBoxPrimitiveComponent>();
	m_pBboxComponent->m_size = Vec3(1.6f, 1.6f, 3.f);
	m_pBboxComponent->m_bReactToCollisions = true;

	//DecalComponent(Placement) Initialization
	m_pDecalComponent = m_pEntity->CreateComponent<Cry::DefaultComponents::CDecalComponent>();
	m_pDecalComponent->SetTransformMatrix(Matrix34::Create(Vec3(2.0f, 3.6f, 3), IDENTITY, Vec3(0.14f, -1.7f, 0)));
	m_pDecalComponent->SetMaterialFileName(BUILDING_PLACEMENT_GREEN_DECAL_MATERIAL);
	m_pDecalComponent->SetSortPriority(50);
	m_pDecalComponent->SetDepth(10);
	m_pDecalComponent->Spawn();

	//BuildingComponent initialization
	m_pBuildingComponent = m_pEntity->GetOrCreateComponent<BuildingComponent>();
	m_pBuildingComponent->SetPathToTrussMesh(GUNPOWDER_FACTORY_1_TRUSS_MODEL_PATH);
	SBuildingInfo buildingInfo;
	buildingInfo.m_populationProduces = 0;
	m_pBuildingComponent->SetBuildingInfo(buildingInfo);
	//m_pBuildingComponent->SetMaxHealth(700.f);
	//UIItems

	//Update bounding box
	AABB aabb;
	m_pEntity->GetLocalBounds(aabb);
	Vec3 min = Vec3(aabb.min.x - 1.8f, aabb.min.y - 5.0f, aabb.min.z);
	Vec3 max = Vec3(aabb.max.x + 1.3f, aabb.max.y + 1.5f, aabb.max.z);
	AABB newAABB = AABB(min, max);
	m_pEntity->SetLocalBounds(newAABB, true);

	//CostComponent Initializations
	m_pCostComponent = m_pEntity->GetOrCreateComponent<CostComponent>();
	m_pCostComponent->SetCost(GunPowderFactory1BuildingComponent::GetCost());

	//WorkplaceComponent  Initializations
	m_pWorkplaceComponent = m_pEntity->GetOrCreateComponent<WorkplaceComponent>();
	m_pWorkplaceComponent->SetMaxWorkersCount(1);

	//WorkPositionAttachment
	m_pParticleComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CParticleComponent>();
	m_pParticleComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0, 0, 6)));
	m_pParticleComponent->SetEffectName("Objects/effects/smoke/smoke_particle_1.pfx");
	m_pParticleComponent->Activate(false);

	//WorkPositionAttachment
	m_pWorkPositionAttachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("workPosition1");
}


Cry::Entity::EventFlags GunPowderFactory1BuildingComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void GunPowderFactory1BuildingComponent::ProcessEvent(const SEntityEvent& event)
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


void GunPowderFactory1BuildingComponent::UpdateAssignedWorkers()
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
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "GunPowderFactory1BuildingComponent:(UpdateCurrentMoveToAttachment) pAIController is null");
		return;
	}
	ResourceCollectorComponent* pResourceCollectorComponent = pWorker->GetComponent<ResourceCollectorComponent>();
	if (!pResourceCollectorComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "GunPowderFactory1BuildingComponent:(UpdateCurrentMoveToAttachment) pResourceCollectorComponent is null");
		return;
	}
	WorkerComponent* pWorkerComponent = pWorker->GetComponent<WorkerComponent>();
	if (!pWorkerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "GunPowderFactory1BuildingComponent:(UpdateCurrentMoveToAttachment) pWorkerComponent is null");
		return;
	}
	if (!m_pWorkplaceComponent->GetWorkers()[0] || m_pWorkplaceComponent->GetWorkers()[0]->IsGarbage()) {
		return;
	}

	int32 SulfurRequestAmount = 30;
	int32 WoodRequestAmount = 30;
	int32 GunPowderProducedAmount = 10;

	//**********************************Move to Warehouse and pickup some Sulfur
	if (!bIsCollectedSulfur) {

		SResourceInfo pSulfurResourceRequest;
		pSulfurResourceRequest.m_sulfurAmount = SulfurRequestAmount;
		if (!pResourceManager->CheckIfResourcesAvailable(pSulfurResourceRequest)) {
			return;
		}
		if (!m_pWarehouseEntity) {
			m_pWarehouseEntity = EntityUtils::FindClosestWarehouse(m_pEntity);
			return;
		}

		Vec3 warehouseExitPoint = m_pWarehouseEntity->GetComponent<BuildingComponent>()->GetExitPoint();
		//Move closer to warehouse if it's not close
		f32 distanceToWareHouse = EntityUtils::GetDistance(m_pWorkplaceComponent->GetWorkers()[0]->GetWorldPos(), warehouseExitPoint, nullptr);
		if (m_pWarehouseEntity && distanceToWareHouse > 1) {
			pAIController->MoveTo(warehouseExitPoint, false);
			pAIController->LookAtWalkDirection();
		}
		//Pickup Sulfur from Warehouse
		else {
			pAIController->StopMoving();
			pAIController->LookAt(m_pWarehouseEntity->GetWorldPos());
			pResourceManager->RequsetResources(pSulfurResourceRequest);
			pResourceCollectorComponent->AddResource(SulfurRequestAmount);
			pResourceCollectorComponent->SetCurrentResourceType(EResourceType::SULFUR);

			bIsCollectedSulfur = true;
		}
	}

	//**********************************Transfer Sulfur to Factory
	if (bIsCollectedSulfur && !bIsCollectedWood && !bIsTransferedSulfurToFactory && !bIsTransferedWoodToFactory) {
		Vec3 workingPoint = m_pWorkPositionAttachment->GetAttWorldAbsolute().t;
		//Move closer to bakery if it's not close
		f32 distanceToBakery = EntityUtils::GetDistance(m_pWorkplaceComponent->GetWorkers()[0]->GetWorldPos(), workingPoint, nullptr);
		if (m_pWarehouseEntity && distanceToBakery > 1) {
			pAIController->MoveTo(workingPoint, false);
			pAIController->LookAtWalkDirection();
			m_workTimePassed = 0;
		}
		//
		else {
			pAIController->StopMoving();
			pAIController->LookAt(m_pEntity->GetWorldPos());
			pResourceCollectorComponent->EmptyResources();

			//if (m_workTimePassed >= m_timeBetweenWorks) {
				bIsTransferedSulfurToFactory = true;

				//pResourceCollectorComponent->AddResource(GunPowderProducedAmount);
				//pResourceCollectorComponent->SetCurrentResourceType(EResourceType::GUN_POWDER);
		//	}
		}
	}

	//**********************************Move to Warehouse and pickup some Wood
	if (!bIsCollectedWood && bIsCollectedSulfur && bIsTransferedSulfurToFactory && !bIsTransferedWoodToFactory) {

		SResourceInfo pWoodResourceRequest;
		pWoodResourceRequest.m_woodAmount = WoodRequestAmount;
		if (!pResourceManager->CheckIfResourcesAvailable(pWoodResourceRequest)) {
			return;
		}
		if (!m_pWarehouseEntity) {
			m_pWarehouseEntity = EntityUtils::FindClosestWarehouse(m_pEntity);
			return;
		}

		Vec3 warehouseExitPoint = m_pWarehouseEntity->GetComponent<BuildingComponent>()->GetExitPoint();
		//Move closer to warehouse if it's not close
		f32 distanceToWareHouse = EntityUtils::GetDistance(m_pWorkplaceComponent->GetWorkers()[0]->GetWorldPos(), warehouseExitPoint, nullptr);
		if (m_pWarehouseEntity && distanceToWareHouse > 1) {
			pAIController->MoveTo(warehouseExitPoint, false);
			pAIController->LookAtWalkDirection();
		}
		//Pickup Sulfur from Warehouse
		else {
			pAIController->StopMoving();
			pAIController->LookAt(m_pWarehouseEntity->GetWorldPos());
			pResourceManager->RequsetResources(pWoodResourceRequest);
			pResourceCollectorComponent->AddResource(WoodRequestAmount);
			pResourceCollectorComponent->SetCurrentResourceType(EResourceType::WOOD);

			bIsCollectedWood = true;
		}
	}

	//**********************************Transfer Wood to Factory And Create GunPowder
	if (bIsCollectedSulfur && bIsCollectedWood && bIsTransferedSulfurToFactory && !bIsTransferedWoodToFactory) {
		Vec3 workingPoint = m_pWorkPositionAttachment->GetAttWorldAbsolute().t;
		//Move closer to Factory if it's not close
		f32 distanceToBakery = EntityUtils::GetDistance(m_pWorkplaceComponent->GetWorkers()[0]->GetWorldPos(), workingPoint, nullptr);
		if (m_pWarehouseEntity && distanceToBakery > 1) {
			pAIController->MoveTo(workingPoint, false);
			pAIController->LookAtWalkDirection();
			m_workTimePassed = 0;
		}
		//
		else {
			pAIController->StopMoving();
			pAIController->LookAt(m_pEntity->GetWorldPos());
			pResourceCollectorComponent->EmptyResources();
			m_pParticleComponent->Activate(true);

			if (m_workTimePassed >= m_timeBetweenWorks) {
				bIsTransferedWoodToFactory = true;

				pResourceCollectorComponent->AddResource(GunPowderProducedAmount);
				pResourceCollectorComponent->SetCurrentResourceType(EResourceType::GUN_POWDER);
				m_pParticleComponent->Activate(false);
			}
		}
	}

	//**********************************Transfer GunPowder to warehouse
	if (bIsCollectedSulfur && bIsCollectedWood && bIsTransferedSulfurToFactory && bIsTransferedWoodToFactory) {
		if (!m_pWarehouseEntity) {
			m_pWarehouseEntity = EntityUtils::FindClosestWarehouse(m_pEntity);
			return;
		}

		Vec3 warehouseExitPoint = m_pWarehouseEntity->GetComponent<BuildingComponent>()->GetExitPoint();
		//Move closer to warehouse if it's not close
		f32 distanceToWareHouse = EntityUtils::GetDistance(m_pWorkplaceComponent->GetWorkers()[0]->GetWorldPos(), warehouseExitPoint, nullptr);
		if (m_pWarehouseEntity && distanceToWareHouse > 1) {
			pAIController->MoveTo(warehouseExitPoint, false);
			pAIController->LookAtWalkDirection();
		}
		//Transer GunPowder to Warehouse
		else {
			pAIController->StopMoving();
			pAIController->LookAt(m_pWarehouseEntity->GetWorldPos());
			pResourceManager->AddResource(EResourceType::GUN_POWDER, GunPowderProducedAmount);
			pResourceCollectorComponent->EmptyResources();

			bIsCollectedSulfur = false;
			bIsTransferedSulfurToFactory = false;
			bIsCollectedWood = false;
			bIsTransferedWoodToFactory = false;
			pWorkerComponent->SetHasEnteredWorkplace(false);
		}
	}
}

SResourceInfo GunPowderFactory1BuildingComponent::GetCost()
{
	SResourceInfo cost;
	cost.m_moneyAmount = 180;
	cost.m_oilAmount = 70;
	cost.m_populationAmount = 2;
	cost.m_woodAmount = 60;
	return cost;
}