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
	m_pDecalComponent->SetTransformMatrix(Matrix34::Create(Vec3(2.5f, 3.6f, 3), IDENTITY, Vec3(0.7f, -1.7f, 0)));
	m_pDecalComponent->SetMaterialFileName(BUILDING_PLACEMENT_GREEN_DECAL_MATERIAL);
	m_pDecalComponent->SetSortPriority(50);
	m_pDecalComponent->SetDepth(10);
	m_pDecalComponent->Spawn();

	//BuildingComponent initialization
	SetPathToTrussMesh(GUNPOWDER_FACTORY_1_TRUSS_MODEL_PATH);
	SBuildingInfo buildingInfo;
	buildingInfo.m_populationProduces = 0;
	SetBuildingInfo(buildingInfo);
	SetImagePath(GunPowderFactory1BuildingComponent::GetDescription().sIcon);
	//m_pBuildingComponent->SetMaxHealth(700.f);
	//UIItems

	//Update bounding box
	AABB aabb;
	m_pEntity->GetLocalBounds(aabb);
	Vec3 min = Vec3(aabb.min.x - 1.8f, aabb.min.y - 5.0f, aabb.min.z);
	Vec3 max = Vec3(aabb.max.x + 2.3f, aabb.max.y + 1.5f, aabb.max.z);
	AABB newAABB = AABB(min, max);
	m_pEntity->SetLocalBounds(newAABB, true);

	//OwnerComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetComponent<OwnerInfoComponent>();

	//CostComponent Initializations
	m_pCostComponent = m_pEntity->GetOrCreateComponent<CostComponent>();
	m_pCostComponent->SetCost(GunPowderFactory1BuildingComponent::GetDescription().price);

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

	//ParticleComponent Initialization
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


void GunPowderFactory1BuildingComponent::UpdateAssignedWorkers()
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
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "GunPowderFactory1BuildingComponent:(UpdateCurrentMoveToAttachment) pWorkerComponent is null");
		return;
	}
	if (!pWorkerComponent->HasEnteredWorkplace()) {
		return;
	}

	int32 productionWaitAmount = 6;
	int32 SulfurRequestAmount = 10;
	int32 WoodRequestAmount = 30;
	int32 GunPowderProducedAmount = 10;
	Vec3 workPosition = m_pWorkPositionAttachment->GetAttWorldAbsolute().t;

	//**********************************Move to Warehouse and pickup some Sulfur And Transfer To Factory
	if (!bIsCollectedSulfurAndTransferedToFactory) {
		if (pWorkerComponent->PickResourceFromWarehouseAndTransferToPosition(EResourceType::SULFUR, SulfurRequestAmount, workPosition)) {
			bIsCollectedSulfurAndTransferedToFactory = true;
		}
	}

	//**********************************Move to Warehouse and pickup some Wood And Transfer To Factory
	if (bIsCollectedSulfurAndTransferedToFactory && !bIsCollectedWoodAndTransferedToFactory) {
		if (pWorkerComponent->PickResourceFromWarehouseAndTransferToPosition(EResourceType::WOOD, WoodRequestAmount, workPosition)) {
			bIsCollectedWoodAndTransferedToFactory = true;
			m_pParticleComponent->Activate(true);
		}
	}

	//**********************************Produce GunPowder
	if (bIsCollectedSulfurAndTransferedToFactory && bIsCollectedWoodAndTransferedToFactory && !bIsProducedGunPowder) {
		if (pWorkerComponent->WaitAndPickResources(productionWaitAmount, workPosition, workPosition, EResourceType::GUN_POWDER, GunPowderProducedAmount)) {
			bIsProducedGunPowder = true;
			m_pParticleComponent->Activate(false);
		}
	}

	//**********************************Transfer GunPowder to warehouse
	if (bIsCollectedSulfurAndTransferedToFactory && bIsCollectedWoodAndTransferedToFactory && bIsProducedGunPowder) {
		if (pWorkerComponent->TransferResourcesToWarehouse(EResourceType::GUN_POWDER, GunPowderProducedAmount)) {
			bIsCollectedSulfurAndTransferedToFactory = false;
			bIsCollectedWoodAndTransferedToFactory = false;
			bIsProducedGunPowder = false;
			pWorkerComponent->SetHasEnteredWorkplace(false);
		}
	}
}

SDescription GunPowderFactory1BuildingComponent::GetDescription()
{
	SResourceInfo price;
	price.m_moneyAmount = 300;
	price.m_oilAmount = 600;
	price.m_ironAmount = 800;
	price.m_woodAmount = 300;

	SDescription m_pDescription;
	m_pDescription.sName = "GunPowder Factory 1";
	m_pDescription.sDescription = "GunPowder Factory 1 Building.";
	m_pDescription.sBuyDescription = "Build GunPowder Factory 1 Building.";
	m_pDescription.price = price;
	m_pDescription.sIcon = "gun_powder_factory_1_icon.png";

	return m_pDescription;
}