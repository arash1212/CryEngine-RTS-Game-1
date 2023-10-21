#include "StdAfx.h"
#include "Farm1Building.h"
#include "GamePlugin.h"

#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Controller/AIController.h>
#include <Components/Weapons/BaseWeapon.h>
#include <Components/Selectables/UnitAnimation.h>
#include <Components/Managers/ActionManager.h>
#include <Components/Selectables/ResourceCollector.h>
#include <Components/Selectables/ResourceStorage.h>

#include <Components/Selectables/Selectable.h>
#include <UIItems/IBaseUIItem.h>
#include <UIItems/Items/UICancelItem.h>
#include <UIItems/Items/UIChangeStanceItem.h>
#include <UIItems/Items/Buildings/UIHQ1BuildItem.h>
#include<UIItems/Items/Buildings/TrainUnits/UITrainEngineer1Item.h>
#include<UIItems/Items/Buildings/TrainUnits/UITrainSoldier1Item.h>
#include <Components/Selectables/Worker.h>

#include <Components/BaseBuilding/Building.h>
#include <Utils/MathUtils.h>
#include <Components/Selectables/Cost.h>

#include <Components/Selectables/Workplace.h>

#include <Components/Managers/ResourceManager.h>

#include <Utils/EntityUtils.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


namespace
{
	static void RegisterFarm1BuildingComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Farm1BuildingComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterFarm1BuildingComponent);
}


void Farm1BuildingComponent::Initialize()
{
	//AnimationComponent Initializations
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0, 0, 0)));
	m_pAnimationComponent->SetCharacterFile(FARM_BUILDING_1_MODEL_PATH);
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/farm1.adb");
	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Idle");
	m_pAnimationComponent->SetAnimationDrivenMotion(false);
	m_pAnimationComponent->LoadFromDisk();
	m_pAnimationComponent->ResetCharacter();

	//BoxComponent Initialization
	m_pBboxComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CBoxPrimitiveComponent>();
	m_pBboxComponent->m_size = Vec3(5.9f, 2.7f, 1.3f);
	m_pBboxComponent->m_bReactToCollisions = true;

	//DecalComponent(Placement) Initialization
	m_pDecalComponent = m_pEntity->CreateComponent<Cry::DefaultComponents::CDecalComponent>();
	m_pDecalComponent->SetTransformMatrix(Matrix34::Create(Vec3(8.2f, 4.1f, 3), IDENTITY, Vec3(0.5f, 0, 0)));
	m_pDecalComponent->SetMaterialFileName(BUILDING_PLACEMENT_GREEN_DECAL_MATERIAL);
	m_pDecalComponent->SetSortPriority(50);
	m_pDecalComponent->SetDepth(10);
	m_pDecalComponent->Spawn();

	//BuildingComponent initialization
	m_pBuildingComponent = m_pEntity->GetOrCreateComponent<BuildingComponent>();
	m_pBuildingComponent->SetPathToTrussMesh(FARM_BUILDING_1_TRUSS_MODEL_PATH);
	SBuildingInfo buildingInfo;
	buildingInfo.m_populationProduces = 0;
	m_pBuildingComponent->SetBuildingInfo(buildingInfo);
	//UIItems

	//Update bounding box
	AABB aabb;
	m_pEntity->GetLocalBounds(aabb);
	Vec3 min = Vec3(aabb.min.x - 7.f, aabb.min.y - 9.1f, aabb.min.z);
	Vec3 max = Vec3(aabb.max.x + 12.f, aabb.max.y + 11.1f, aabb.max.z);
	AABB newAABB = AABB(min, max);
	m_pEntity->SetLocalBounds(newAABB, true);

	//CostComponent Initializations
	m_pCostComponent = m_pEntity->GetOrCreateComponent<CostComponent>();
	m_pCostComponent->SetCost(Barracks1BuildingComponent::GetCost());

	//Wheat positions
	IAttachment* m_pWheat1Attachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("wheatPos1");
	m_pWheat1Attachment->HideAttachment(true);
	m_pWheatAttachments.append(m_pWheat1Attachment);

	IAttachment* m_pWheat2Attachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("wheatPos2");
	m_pWheat2Attachment->HideAttachment(true);
	m_pWheatAttachments.append(m_pWheat2Attachment);

	IAttachment* m_pWheat3Attachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("wheatPos3");
	m_pWheat3Attachment->HideAttachment(true);
	m_pWheatAttachments.append(m_pWheat3Attachment);

	IAttachment* m_pWheat4Attachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("wheatPos4");
	m_pWheat4Attachment->HideAttachment(true);
	m_pWheatAttachments.append(m_pWheat4Attachment);

	IAttachment* m_pWheat5Attachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("wheatPos5");
	m_pWheat5Attachment->HideAttachment(true);
	m_pWheatAttachments.append(m_pWheat5Attachment);

	//Wheat Sprout positions
	IAttachment* m_pWheatSprout1Attachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("wheatSprout1");
	m_pWheatSprout1Attachment->HideAttachment(true);
	m_pWheatSproutAttachments.append(m_pWheatSprout1Attachment);

	IAttachment* m_pWheatSprout2Attachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("wheatSprout2");
	m_pWheatSprout2Attachment->HideAttachment(true);
	m_pWheatSproutAttachments.append(m_pWheatSprout2Attachment);

	IAttachment* m_pWheatSprout3Attachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("wheatSprout3");
	m_pWheatSprout3Attachment->HideAttachment(true);
	m_pWheatSproutAttachments.append(m_pWheatSprout3Attachment);

	IAttachment* m_pWheatSprout4Attachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("wheatSprout4");
	m_pWheatSprout4Attachment->HideAttachment(true);
	m_pWheatSproutAttachments.append(m_pWheatSprout4Attachment);

	IAttachment* m_pWheatSprout5Attachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("wheatSprout5");
	m_pWheatSprout5Attachment->HideAttachment(true);
	m_pWheatSproutAttachments.append(m_pWheatSprout5Attachment);

	//WorkplaceComponent  Initializations
	m_pWorkplaceComponent = m_pEntity->GetOrCreateComponent<WorkplaceComponent>();
	m_pWorkplaceComponent->SetMoveToAttachments(m_pWheatAttachments);
	m_pWorkplaceComponent->SetMaxWorkersCount(1);
}


Cry::Entity::EventFlags Farm1BuildingComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void Farm1BuildingComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		f32 DeltaTime = event.fParam[0];

		UpdateCurrentMoveToAttachment();

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

void Farm1BuildingComponent::UpdateCurrentMoveToAttachment()
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
	if (m_pWorker1CurrentAssignePos == ZERO) {
		m_pWorker1CurrentAssignePos = m_pWheatAttachments[m_currentIndex]->GetAttWorldAbsolute().t;
		m_currentIndex = 0;
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
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "Farm1BuildingComponent:(UpdateCurrentMoveToAttachment) pAIController is null");
		return;
	}
	ResourceCollectorComponent* pResourceCollectorComponent = pWorker->GetComponent<ResourceCollectorComponent>();
	if (!pAIController) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "Farm1BuildingComponent:(UpdateCurrentMoveToAttachment) pResourceCollectorComponent is null");
		return;
	}
	WorkerComponent* pWorkerComponent = pWorker->GetComponent<WorkerComponent>();
	if (!pAIController) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "Farm1BuildingComponent:(UpdateCurrentMoveToAttachment) pWorkerComponent is null");
		return;
	}

	//Plant
	if (!bIsPlantingDone) {
		f32 DistanceToCurrentAssignedPos = EntityUtils::GetDistance(m_pWorker1CurrentAssignePos, pWorker->GetWorldPos(), nullptr);
		if (DistanceToCurrentAssignedPos > 1.f) {
			pAIController->MoveTo(m_pWorker1CurrentAssignePos, false);
			pAIController->LookAtWalkDirection();
			m_workTimePassed = 0;
		}
		else {
			if (m_workTimePassed >= m_timeBetweenWorks) {
				if (m_currentIndex < m_pWheatAttachments.size() - 1) {
					m_pWheatSproutAttachments[m_currentIndex]->HideAttachment(false);

					m_currentIndex++;
					m_pWorker1CurrentAssignePos = m_pWheatAttachments[m_currentIndex]->GetAttWorldAbsolute().t;
				}
				else {
					m_pWheatSproutAttachments[m_currentIndex]->HideAttachment(false);
					m_currentIndex = 0;
					bIsPlantingDone = true;
				}
			}
		}
	}

	//Wait
	if (bIsPlantingDone && !bIsHarvestingStarted || bIsHarvestingDone) {
		f32 DistanceToExitPos = EntityUtils::GetDistance(pWorker->GetWorldPos(), m_pBuildingComponent->GetExitPoint(), nullptr);
		if (DistanceToExitPos > 1.0f) {
			pAIController->MoveTo(m_pWheatAttachments[2]->GetAttWorldAbsolute().t, false);
			pAIController->LookAtWalkDirection();
			m_workTimePassed = -6.f;
		}
		else {
			if (m_workTimePassed >= m_timeBetweenWorks) {
				bIsHarvestingStarted = true;
				m_currentIndex = 0;
				m_pWorker1CurrentAssignePos = m_pWheatAttachments[m_currentIndex]->GetAttWorldAbsolute().t;

				//hide sprouts
				for (IAttachment* sprout : m_pWheatSproutAttachments) {
					sprout->HideAttachment(true);
				}

				//show wheats
				for (IAttachment* wheat : m_pWheatAttachments) {
					wheat->HideAttachment(false);
				}
			}
		}
	}

	//Harvest
	if (bIsPlantingDone && !bIsHarvestingDone && bIsHarvestingStarted) {
		f32 DistanceToCurrentAssignedPos = EntityUtils::GetDistance(m_pWorker1CurrentAssignePos, m_pWorkplaceComponent->GetWorkers()[0]->GetWorldPos(), nullptr);
		if (DistanceToCurrentAssignedPos > 1.f) {
			pAIController->MoveTo(m_pWorker1CurrentAssignePos, false);
			pAIController->LookAtWalkDirection();
			m_workTimePassed = 0;
		}
		else {
			if (m_workTimePassed >= m_timeBetweenWorks) {
				if (m_currentIndex < m_pWheatAttachments.size() - 1) {
					m_pWheatAttachments[m_currentIndex]->HideAttachment(true);

					m_currentIndex++;
					m_pWorker1CurrentAssignePos = m_pWheatAttachments[m_currentIndex]->GetAttWorldAbsolute().t;

					pResourceCollectorComponent->SetCurrentResourceType(EResourceType::WHEAT);
					pResourceCollectorComponent->AddResource(pResourceCollectorComponent->GetMaxResourceCanBeCollected() / m_pWheatAttachments.size());

				}
				else {
					pResourceCollectorComponent->SetCurrentResourceType(EResourceType::WHEAT);
					pResourceCollectorComponent->AddResource(pResourceCollectorComponent->GetMaxResourceCanBeCollected() / m_pWheatAttachments.size());

					m_pWheatAttachments[m_currentIndex]->HideAttachment(true);
					bIsHarvestingDone = true;
					m_currentIndex = 0;
				}
			}
		}
	}

	//SendResource to warehouse
	if (bIsPlantingDone && bIsHarvestingDone && bIsHarvestingStarted) {
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
		//Deliver Resource to Warehouse
		else {
			pAIController->StopMoving();
			pAIController->LookAt(m_pWarehouseEntity->GetWorldPos());
			pResourceCollectorComponent->SendResourceToWareHouse();

			m_currentIndex = 0;
			bIsPlantingDone = false;
			bIsHarvestingDone = false;
			bIsHarvestingStarted = false;
			pWorkerComponent->SetHasEnteredWorkplace(false);
			m_pWorker1CurrentAssignePos = m_pWheatAttachments[m_currentIndex]->GetAttWorldAbsolute().t;
		}
	}
}

IEntity* Farm1BuildingComponent::FindClosestWarehouse()
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

SResourceInfo Farm1BuildingComponent::GetCost()
{
	SResourceInfo cost;
	cost.m_moneyAmount = 170;
	cost.m_oilAmount = 60;
	cost.m_populationAmount = 5;
	return cost;
}