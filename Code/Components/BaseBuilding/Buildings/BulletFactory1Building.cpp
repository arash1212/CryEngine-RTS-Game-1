#include "StdAfx.h"
#include "BulletFactory1Building.h"
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
	static void RegisterBulletFactory1BuildingComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(BulletFactory1BuildingComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterBulletFactory1BuildingComponent);
}


void BulletFactory1BuildingComponent::Initialize()
{
	//AnimationComponent Initializations
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0, 0, 0)));
	m_pAnimationComponent->SetCharacterFile(BULLET_FACTORY_BUILDING_1_MODEL_PATH);
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/bulletfactory1.adb");
	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Idle");
	m_pAnimationComponent->SetAnimationDrivenMotion(false);
	m_pAnimationComponent->LoadFromDisk();
	m_pAnimationComponent->ResetCharacter();

	//BoxComponent Initialization
	m_pBboxComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CBoxPrimitiveComponent>();
	m_pBboxComponent->m_size = Vec3(5.f, 2.7f, 2.5f);
	m_pBboxComponent->m_bReactToCollisions = true;

	//DecalComponent(Placement) Initialization
	m_pDecalComponent = m_pEntity->CreateComponent<Cry::DefaultComponents::CDecalComponent>();
	m_pDecalComponent->SetTransformMatrix(Matrix34::Create(Vec3(5.3f, 3.9f, 3), IDENTITY, Vec3(0.06f, -1.2f, 0)));
	m_pDecalComponent->SetMaterialFileName(BUILDING_PLACEMENT_GREEN_DECAL_MATERIAL);
	m_pDecalComponent->SetSortPriority(50);
	m_pDecalComponent->SetDepth(10);
	m_pDecalComponent->Spawn();

	//BuildingComponent initialization
	m_pBuildingComponent = m_pEntity->GetOrCreateComponent<BuildingComponent>();
	m_pBuildingComponent->SetPathToTrussMesh(BULLET_FACTORY_1_TRUSS_MODEL_PATH);
	SBuildingInfo buildingInfo;
	buildingInfo.m_populationProduces = 0;
	m_pBuildingComponent->SetBuildingInfo(buildingInfo);
	m_pBuildingComponent->SetImagePath(BulletFactory1BuildingComponent::GetDescription().sIcon);
	//m_pBuildingComponent->SetMaxHealth(700.f);
	//UIItems

	//Update bounding box
	AABB aabb;
	m_pEntity->GetLocalBounds(aabb);
	Vec3 min = Vec3(aabb.min.x - 3.5f, aabb.min.y - 5.0f, aabb.min.z - 4.628f);
	Vec3 max = Vec3(aabb.max.x + 3.8f, aabb.max.y + 2.5f, aabb.max.z + 3);
	AABB newAABB = AABB(min, max);
	m_pEntity->SetLocalBounds(newAABB, true);

	//CostComponent Initializations
	m_pCostComponent = m_pEntity->GetOrCreateComponent<CostComponent>();
	m_pCostComponent->SetCost(BulletFactory1BuildingComponent::GetDescription().cost);

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


Cry::Entity::EventFlags BulletFactory1BuildingComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void BulletFactory1BuildingComponent::ProcessEvent(const SEntityEvent& event)
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


void BulletFactory1BuildingComponent::UpdateAssignedWorkers()
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
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "BulletFactory1BuildingComponent:(UpdateCurrentMoveToAttachment) pWorkerComponent is null");
		return;
	}
	if (!pWorkerComponent->HasEnteredWorkplace()) {
		return;
	}

	int32 productionWaitAmount = 5;
	int32 GunPowderRequestAmount = 30;
	int32 IronRequestAmount = 30;
	int32 BulletProducedAmount = 10;
	Vec3 workPosition = m_pWorkPositionAttachment->GetAttWorldAbsolute().t;

	//**********************************Move to Warehouse and pickup some GunPowder And Transfer To Factory
	if (!bIsCollectedGunPowderAndTransferedToFactory) {
		if (pWorkerComponent->PickResourceFromWarehouseAndTransferToPosition(EResourceType::GUN_POWDER, GunPowderRequestAmount, workPosition)) {
			bIsCollectedGunPowderAndTransferedToFactory = true;
		}
	}

	//**********************************Move to Warehouse and pickup some Iron And Transfer To Factory
	if (bIsCollectedGunPowderAndTransferedToFactory && !bIsCollectedIronAndTransferedToFactory) {
		if (pWorkerComponent->PickResourceFromWarehouseAndTransferToPosition(EResourceType::IRON, IronRequestAmount, workPosition)) {
			bIsCollectedIronAndTransferedToFactory = true;
			m_pParticleComponent->Activate(true);
		}
	}

	//**********************************Produce Bullet
	if (bIsCollectedGunPowderAndTransferedToFactory && bIsCollectedIronAndTransferedToFactory && !bIsProducedBullet) {
		if (pWorkerComponent->WaitAndPickResources(productionWaitAmount, workPosition, workPosition, EResourceType::BULLET, BulletProducedAmount)) {
			bIsProducedBullet = true;
			m_pParticleComponent->Activate(false);
		}
	}

	//**********************************Transfer Bullet to warehouse
	if (bIsCollectedGunPowderAndTransferedToFactory && bIsCollectedIronAndTransferedToFactory && bIsProducedBullet) {
		if (pWorkerComponent->TransferResourcesToWarehouse(EResourceType::BULLET, BulletProducedAmount)) {
			bIsCollectedGunPowderAndTransferedToFactory = false;
			bIsCollectedIronAndTransferedToFactory = false;
			bIsProducedBullet = false;
			pWorkerComponent->SetHasEnteredWorkplace(false);
		}
	}
}

SDescription BulletFactory1BuildingComponent::GetDescription()
{
	SResourceInfo cost;
	cost.m_moneyAmount = 400;
	cost.m_oilAmount = 400;
	cost.m_woodAmount = 900;
	cost.m_ironAmount = 800;

	SDescription m_pDescription;
	m_pDescription.sName = "Bullet Factory 1";
	m_pDescription.sDescription = "Bullet Factory 1 Building.";
	m_pDescription.sBuyDescription = "Build Bullet Factory 1 Building.";
	m_pDescription.cost = cost;
	m_pDescription.sIcon = "bulletfactory_1_icon.png";

	return m_pDescription;
}