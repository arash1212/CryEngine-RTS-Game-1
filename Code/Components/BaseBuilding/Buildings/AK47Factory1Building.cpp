#include "StdAfx.h"
#include "ak47Factory1Building.h"
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
	static void RegisterAK47Factory1BuildingComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(AK47Factory1BuildingComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterAK47Factory1BuildingComponent);
}


void AK47Factory1BuildingComponent::Initialize()
{
	//AnimationComponent Initializations
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0, 0, 0)));
	m_pAnimationComponent->SetCharacterFile(AK47_FACTORY_BUILDING_1_MODEL_PATH);
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/bulletfactory1.adb");
	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Idle");
	m_pAnimationComponent->SetAnimationDrivenMotion(false);
	m_pAnimationComponent->LoadFromDisk();
	m_pAnimationComponent->ResetCharacter();

	//BoxComponent Initialization
	m_pBboxComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CBoxPrimitiveComponent>();
	m_pBboxComponent->m_size = Vec3(4.6f, 2.5f, 1.8f);
	m_pBboxComponent->m_bReactToCollisions = true;

	//DecalComponent(Placement) Initialization
	m_pDecalComponent = m_pEntity->CreateComponent<Cry::DefaultComponents::CDecalComponent>();
	m_pDecalComponent->SetTransformMatrix(Matrix34::Create(Vec3(5.0f, 4.6, 3), IDENTITY, Vec3(0.55f, -1.50f, 0)));
	m_pDecalComponent->SetMaterialFileName(BUILDING_PLACEMENT_GREEN_DECAL_MATERIAL);
	m_pDecalComponent->SetSortPriority(50);
	m_pDecalComponent->SetDepth(10);
	m_pDecalComponent->Spawn();

	//BuildingComponent initialization
	m_pBuildingComponent = m_pEntity->GetOrCreateComponent<BuildingComponent>();
	m_pBuildingComponent->SetPathToTrussMesh(AK47_FACTORY_1_TRUSS_MODEL_PATH);
	SBuildingInfo buildingInfo;
	buildingInfo.m_populationProduces = 0;
	m_pBuildingComponent->SetBuildingInfo(buildingInfo);
	//m_pBuildingComponent->SetMaxHealth(700.f);
	//UIItems

	//Update bounding box
	AABB aabb;
	m_pEntity->GetLocalBounds(aabb);
	Vec3 min = Vec3(aabb.min.x - 1.7f, aabb.min.y - 6.0f, aabb.min.z - 3.17f);
	Vec3 max = Vec3(aabb.max.x + 5.5f, aabb.max.y + 3.0f, aabb.max.z + 2);
	AABB newAABB = AABB(min, max);
	m_pEntity->SetLocalBounds(newAABB, true);

	//CostComponent Initializations
	m_pCostComponent = m_pEntity->GetOrCreateComponent<CostComponent>();
	m_pCostComponent->SetCost(AK47Factory1BuildingComponent::GetCost());

	//WorkplaceComponent  Initializations
	m_pWorkplaceComponent = m_pEntity->GetOrCreateComponent<WorkplaceComponent>();
	m_pWorkplaceComponent->SetMaxWorkersCount(1);

	//WorkPositionAttachment
	m_pWorkPositionAttachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("workPosition1");
}


Cry::Entity::EventFlags AK47Factory1BuildingComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void AK47Factory1BuildingComponent::ProcessEvent(const SEntityEvent& event)
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


void AK47Factory1BuildingComponent::UpdateAssignedWorkers()
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
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "AK47Factory1BuildingComponent:(UpdateCurrentMoveToAttachment) pWorkerComponent is null");
		return;
	}
	if (!pWorkerComponent->HasEnteredWorkplace()) {
		return;
	}

	int32 productionWaitAmount = 7;
	int32 BulletRequestAmount = 5;
	int32 IronRequestAmount = 30;
	int32 AK47ProducedAmount = 1;
	Vec3 workPosition = m_pWorkPositionAttachment->GetAttWorldAbsolute().t;

	//**********************************Move to Warehouse and pickup some Bullet And Transfer To Factory
	if (!bIsPickedupBulletAndTransferedTofactory) {
		if (pWorkerComponent->PickResourceFromWarehouseAndTransferToPosition(EResourceType::BULLET, BulletRequestAmount, workPosition)) {
			bIsPickedupBulletAndTransferedTofactory = true;
		}
	}

	//**********************************Move to Warehouse and pickup some Iron And Transfer To Factory
	if (bIsPickedupBulletAndTransferedTofactory && !bIsPickedupIronAndTransferedTofactory && !bIsProducedAK47) {
		if (pWorkerComponent->PickResourceFromWarehouseAndTransferToPosition(EResourceType::IRON, IronRequestAmount, workPosition)) {
			bIsPickedupIronAndTransferedTofactory = true;
		}
	}

	//**********************************Produce AK47
	if (bIsPickedupBulletAndTransferedTofactory && bIsPickedupIronAndTransferedTofactory && !bIsProducedAK47) {
		if (pWorkerComponent->WaitAndPickResources(productionWaitAmount, workPosition, workPosition, EResourceType::AK47, AK47ProducedAmount)) {
			bIsProducedAK47 = true;
		}
	}

	//**********************************Transfer AK47 to warehouse
	if (bIsPickedupBulletAndTransferedTofactory && bIsPickedupIronAndTransferedTofactory && bIsProducedAK47) {
		if (pWorkerComponent->TransferResourcesToWarehouse(EResourceType::AK47, AK47ProducedAmount)) {
			bIsPickedupBulletAndTransferedTofactory = false;
			bIsPickedupIronAndTransferedTofactory = true;
			bIsProducedAK47 = false;
			pWorkerComponent->SetHasEnteredWorkplace(false);
		}
	}
}

SResourceInfo AK47Factory1BuildingComponent::GetCost()
{
	SResourceInfo cost;
	cost.m_moneyAmount = 50;
	cost.m_oilAmount = 20;
	//cost.m_populationAmount = 2;
	cost.m_woodAmount = 80;
	return cost;
}