#include "StdAfx.h"
#include "Alchemy1Building.h"
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
	static void RegisterAlchemy1BuildingComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(Alchemy1BuildingComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterAlchemy1BuildingComponent);
}


void Alchemy1BuildingComponent::Initialize()
{
	//AnimationComponent Initializations
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0, 0, 0)));
	m_pAnimationComponent->SetCharacterFile(ALCHEMY_BUILDING_1_MODEL_PATH);
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/barracks1.adb");
	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Idle");
	m_pAnimationComponent->SetAnimationDrivenMotion(false);
	m_pAnimationComponent->LoadFromDisk();
	m_pAnimationComponent->ResetCharacter();

	//BoxComponent Initialization
	m_pBboxComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CBoxPrimitiveComponent>();
	m_pBboxComponent->m_size = Vec3(6.f, 4.1f, 2.0f);
	m_pBboxComponent->m_bReactToCollisions = true;

	//DecalComponent(Placement) Initialization
	m_pDecalComponent = m_pEntity->CreateComponent<Cry::DefaultComponents::CDecalComponent>();
	m_pDecalComponent->SetTransformMatrix(Matrix34::Create(Vec3(7.8f, 6.4f, 3), IDENTITY, Vec3(0.3f, 0.6f, 0)));
	m_pDecalComponent->SetMaterialFileName(BUILDING_PLACEMENT_GREEN_DECAL_MATERIAL);
	m_pDecalComponent->SetSortPriority(50);
	m_pDecalComponent->SetDepth(10);
	m_pDecalComponent->Spawn();

	//BuildingComponent initialization
	m_pBuildingComponent = m_pEntity->GetOrCreateComponent<BuildingComponent>();
	m_pBuildingComponent->SetPathToTrussMesh(ALCHEMY_BUILDING_1_TRUSS_MODEL_PATH);
	SBuildingInfo buildingInfo;
	buildingInfo.m_populationProduces = 0;
	m_pBuildingComponent->SetBuildingInfo(buildingInfo);
	//m_pBuildingComponent->SetMaxHealth(700.f);
	//UIItems
	m_pBuildingComponent->AddUIItem(new UITrainEngineer1Item(m_pEntity));

	//Update bounding box
	AABB aabb;
	m_pEntity->GetLocalBounds(aabb);
	Vec3 min = Vec3(aabb.min.x - 5.5f, aabb.min.y - 2.0f, aabb.min.z);
	Vec3 max = Vec3(aabb.max.x + 8.0f, aabb.max.y + 7.0f, aabb.max.z);
	AABB newAABB = AABB(min, max);
	m_pEntity->SetLocalBounds(newAABB, true);

	//CostComponent Initializations
	m_pCostComponent = m_pEntity->GetOrCreateComponent<CostComponent>();
	m_pCostComponent->SetCost(Alchemy1BuildingComponent::GetCost());

	//WorkplaceComponent  Initializations
	m_pWorkplaceComponent = m_pEntity->GetOrCreateComponent<WorkplaceComponent>();
	m_pWorkplaceComponent->SetMaxWorkersCount(1);

	//WorkPositionAttachment
	m_pWorkPositionAttachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("workPosition1");
}


Cry::Entity::EventFlags Alchemy1BuildingComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void Alchemy1BuildingComponent::ProcessEvent(const SEntityEvent& event)
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


void Alchemy1BuildingComponent::UpdateAssignedWorkers()
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
	int32 OilRequestAmount = 30;
	int32 SulfurProducedAmount = 10;
	Vec3 workPosition = m_pBuildingComponent->GetExitPoint();

	//**********************************Move to Warehouse and pickup some Oil
	if (!bIsCollectedOil) {
		if (pWorkerComponent->PickResourceFromWareHouse(EResourceType::OIL, OilRequestAmount)) {
			bIsCollectedOil = true;
		}
	}

	//**********************************Transfer oil to Alchemy
	if (bIsCollectedOil && !bIsTransferedOilToAlchemy) {
		if (pWorkerComponent->TransferResourcesToPosition(workPosition)) {
			bIsTransferedOilToAlchemy = true;
		}
	}

	//**********************************Produce Sulfur
	if (bIsCollectedOil && bIsTransferedOilToAlchemy && !bIsProducedSulfur) {
		if (pWorkerComponent->WaitAndPickResources(productionWaitAmount, m_pEntity->GetWorldPos(), EResourceType::SULFUR, SulfurProducedAmount)) {
			bIsProducedSulfur = true;
		}
	}

	//**********************************Transfer Sulfur to warehouse Flour
	if (bIsCollectedOil && bIsTransferedOilToAlchemy && bIsProducedSulfur) {
		if (pWorkerComponent->TransferResourcesToWarehouse(EResourceType::SULFUR, SulfurProducedAmount)) {
			bIsCollectedOil = false;
			bIsTransferedOilToAlchemy = false;
			bIsProducedSulfur = false;
			pWorkerComponent->SetHasEnteredWorkplace(false);
		}
	}
}

SResourceInfo Alchemy1BuildingComponent::GetCost()
{
	SResourceInfo cost;
	cost.m_moneyAmount = 40;
	cost.m_oilAmount = 30;
	//cost.m_populationAmount = 2;
	cost.m_woodAmount = 60;
	return cost;
}