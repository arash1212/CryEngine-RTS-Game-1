#include "StdAfx.h"
#include "Worker.h"
#include "GamePlugin.h"

#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Controller/AIController.h>
#include <Components/Weapons/BaseWeapon.h>
#include <Components/Selectables/UnitAnimation.h>
#include <Components/Managers/ActionManager.h>
#include <Components/BaseBuilding/Building.h>

#include <Components/Controller/AIController.h>
#include <Components/BaseBuilding/Building.h>

#include <Components/Managers/ResourceManager.h>

#include <Components/Selectables/Worker.h>
#include <Components/Selectables/Workplace.h>
#include <Components/Selectables/ResourceCollector.h>
#include <Components/Selectables/ResourceStorage.h>

#include <Utils/MathUtils.h>
#include <Utils/EntityUtils.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


namespace
{
	static void RegisterWorkerComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(WorkerComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterWorkerComponent);
}


void WorkerComponent::Initialize()
{
	//AIController Initialization
	m_pAIController = m_pEntity->GetOrCreateComponent<AIControllerComponent>();

	//ResourceCollectorComponent Initialization
	m_pResourceCollectorComponent = m_pEntity->GetOrCreateComponent<ResourceCollectorComponent>();

	//OwnerComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetComponent<OwnerInfoComponent>();

	//ResourceManagerComponent Initialization
	m_pResourceManagerComponent = m_pOwnerInfoComponent->GetOwner()->GetComponent<ResourceManagerComponent>();
}


Cry::Entity::EventFlags WorkerComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void WorkerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];

		EnterAssignedWorkplace();

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

void WorkerComponent::EnterAssignedWorkplace()
{
	if (!m_pAssignedWorkplace) {
		return;
	}
	if (bIsEnteredWorkplace) {
		return;
	}
	BuildingComponent* workplace = m_pAssignedWorkplace->GetComponent<BuildingComponent>();
	if (!workplace) {
		return;
	}

	f32 distanceToWorkplaceEnterance = EntityUtils::GetDistance(m_pEntity->GetWorldPos(), workplace->GetExitPoint(), nullptr);
	if (distanceToWorkplaceEnterance > 1.f) {
		m_pAIController->MoveTo(workplace->GetExitPoint(), true);
		m_pAIController->LookAtWalkDirection();
	}
	else {
		bIsEnteredWorkplace = true;
	}
}

void WorkerComponent::AssignWorkplace(IEntity* workplace)
{
	WorkplaceComponent* workplaceComponent = workplace->GetComponent<WorkplaceComponent>();
	if (!workplaceComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "WorkerComponent : (AssignWorkplace) Selected Entity is not a workplace.");
		return;
	}
	this->m_pAssignedWorkplace = workplace;
}

IEntity* WorkerComponent::GetAssignedWorkplace()
{
	return m_pAssignedWorkplace;
}

void WorkerComponent::CancelAssignedWorkplace()
{
	this->bIsEnteredWorkplace = false;
	this->bIsReachedAssignedPoint = false;
	this->m_pAssignedWorkplace = nullptr;
	this->m_pAIController->StopMoving();
}

bool WorkerComponent::HasEnteredWorkplace()
{
	if (!m_pAssignedWorkplace) {
		return false;
	}

	return bIsEnteredWorkplace;
}

void WorkerComponent::SetHasEnteredWorkplace(bool hasEntered)
{
	this->bIsEnteredWorkplace = hasEntered;
}

bool WorkerComponent::HasReachedAssignedPoint()
{
	return bIsReachedAssignedPoint;
}

bool WorkerComponent::PickResourceFromWareHouse(EResourceType resourceType, int32 amount)
{
	SResourceInfo pResourceRequest = GetResourceRequestParams(resourceType, amount);
	if (!m_pResourceManagerComponent->CheckIfResourcesAvailable(pResourceRequest)) {
		return false;
	}
	IEntity* m_pWarehouseEntity = EntityUtils::FindClosestWarehouse(m_pEntity);
	if (!m_pWarehouseEntity) {
		return false;
	}

	Vec3 warehouseExitPoint = m_pWarehouseEntity->GetComponent<BuildingComponent>()->GetExitPoint();
	//Move closer to warehouse if it's not close
	f32 distanceToWareHouse = EntityUtils::GetDistance(m_pEntity->GetWorldPos(), warehouseExitPoint, nullptr);
	if (m_pWarehouseEntity && distanceToWareHouse > 1) {
		m_pAIController->MoveTo(warehouseExitPoint, false);
		m_pAIController->LookAtWalkDirection();
	}
	//Pickup Resource from Warehouse
	else {
		m_pAIController->StopMoving();
		m_pAIController->LookAt(m_pWarehouseEntity->GetWorldPos());
		m_pResourceManagerComponent->RequsetResources(pResourceRequest);
		m_pResourceCollectorComponent->AddResource(amount);
		m_pResourceCollectorComponent->SetCurrentResourceType(resourceType);

		return true;
	}

	return false;
}

bool WorkerComponent::TransferResourcesToPosition(Vec3 position)
{
	//Move closer to Workplace if it's not close
	f32 distanceToWorkplace = EntityUtils::GetDistance(m_pEntity->GetWorldPos(), position, nullptr);
	if (distanceToWorkplace > 1) {
		m_pAIController->MoveTo(position, false);
		m_pAIController->LookAtWalkDirection();
	}
	//Transfer resources if close to position
	else {
		m_pAIController->StopMoving();
		m_pAIController->LookAt(position);
		m_pResourceCollectorComponent->EmptyResources();
		return true;
	}
	return false;
}

bool WorkerComponent::WaitAndPickResources(int32 waitAmount, Vec3 lookAtPos, EResourceType resourceType, int32 amount)
{	//Timers
	if (m_workTimePassed < waitAmount) {
		m_workTimePassed += 0.5f * gEnv->pTimer->GetFrameTime();
		m_pAIController->LookAt(lookAtPos);
		return false;
	}
	if (m_workTimePassed >= waitAmount) {
		m_pResourceCollectorComponent->AddResource(amount);
		m_pResourceCollectorComponent->SetCurrentResourceType(resourceType);
		m_workTimePassed = 0;
		return true;
	}
	return false;
}

bool WorkerComponent::TransferResourcesToWarehouse(EResourceType resourceType, int32 amount)
{
	IEntity* m_pWarehouseEntity = EntityUtils::FindClosestWarehouse(m_pEntity);
	if (!m_pWarehouseEntity) {
		return false;
	}

	Vec3 warehouseExitPoint = m_pWarehouseEntity->GetComponent<BuildingComponent>()->GetExitPoint();
	//Move closer to warehouse if it's not close
	f32 distanceToWareHouse = EntityUtils::GetDistance(m_pEntity->GetWorldPos(), warehouseExitPoint, nullptr);
	if (m_pWarehouseEntity && distanceToWareHouse > 1) {
		m_pAIController->MoveTo(warehouseExitPoint, false);
		m_pAIController->LookAtWalkDirection();
	}
	//Transer Resource to Warehouse
	else {
		m_pAIController->StopMoving();
		m_pAIController->LookAt(m_pWarehouseEntity->GetWorldPos());
		m_pResourceManagerComponent->AddResource(resourceType, amount);
		m_pResourceCollectorComponent->EmptyResources();

		return true;
	}
	return false;
}

SResourceInfo WorkerComponent::GetResourceRequestParams(EResourceType resourceType, int32 amount)
{
	SResourceInfo pResourceRequest;
	if (resourceType == EResourceType::AK47) {
		pResourceRequest.m_ak47Amount = amount;
	}
	else if (resourceType == EResourceType::BREAD) {
		pResourceRequest.m_breadAmount = amount;
	}
	else if (resourceType == EResourceType::BULLET) {
		pResourceRequest.m_bulletAmount = amount;
	}
	else if (resourceType == EResourceType::FLOUR) {
		pResourceRequest.m_flourAmount = amount;
	}
	else if (resourceType == EResourceType::GUN_POWDER) {
		pResourceRequest.m_gunPowderAmount = amount;
	}
	else if (resourceType == EResourceType::IRON) {
		pResourceRequest.m_ironAmount = amount;
	}
	else if (resourceType == EResourceType::IRON) {
		pResourceRequest.m_ironAmount = amount;
	}
	else if (resourceType == EResourceType::Money) {
		pResourceRequest.m_moneyAmount = amount;
	}
	else if (resourceType == EResourceType::OIL) {
		pResourceRequest.m_oilAmount = amount;
	}
	else if (resourceType == EResourceType::POPULATION) {
		pResourceRequest.m_populationAmount = amount;
	}
	else if (resourceType == EResourceType::SULFUR) {
		pResourceRequest.m_sulfurAmount = amount;
	}
	else if (resourceType == EResourceType::WHEAT) {
		pResourceRequest.m_wheatAmount = amount;
	}
	else if (resourceType == EResourceType::WOOD) {
		pResourceRequest.m_woodAmount = amount;
	}
	return pResourceRequest;
}