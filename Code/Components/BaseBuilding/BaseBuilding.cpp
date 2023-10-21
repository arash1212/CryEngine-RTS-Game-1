#include "StdAfx.h"
#include "BaseBuilding.h"
#include "GamePlugin.h"

#include <CryEntitySystem/IEntitySystem.h>
#include <CryAISystem/INavigationSystem.h>
#include <CryAISystem/NavigationSystem/INavigationUpdatesManager.h>

#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Controller/AIController.h>
#include <Components/Weapons/BaseWeapon.h>
#include <Components/Selectables/UnitAnimation.h>
#include <Components/Managers/ActionManager.h>

#include <Utils/EntityUtils.h>
#include <Utils/MathUtils.h>
#include <Utils/MouseUtils.h>

#include <Components/Info/OwnerInfo.h>
#include <Components/BaseBuilding/Building.h>
#include <Components/BaseBuilding/Buildings/HQ1Building.h>
#include <Components/Managers/ResourceManager.h>
#include <Components/Selectables/Cost.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


namespace
{
	static void RegisterBaseBuildingComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(BaseBuildingComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterBaseBuildingComponent);
}


void BaseBuildingComponent::Initialize()
{
	//AudioComponent Initialization
	m_pAudioComponent = m_pEntity->GetOrCreateComponent<IEntityAudioComponent>();

	//Sounds
	m_buildingPlacementSound = CryAudio::StringToId("Building_Placement_Sound");

	//OwnerInfoComponent initialization
	m_pOwnerInfoComponent = m_pEntity->GetOrCreateComponent<OwnerInfoComponent>();
	m_pOwnerInfoComponent->SetIsPlayer(true);
}


Cry::Entity::EventFlags BaseBuildingComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void BaseBuildingComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];

		UpdateBuildingPosition();

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

void BaseBuildingComponent::UpdateBuildingPosition()
{
	if (!m_pBuildingEntity || MouseUtils::GetActorUnderCursor() || !MouseUtils::IsMouseInsideViewPort()) {
		return;
	}

	Vec3 mousePos = MouseUtils::GetPositionUnderCursor();
	mousePos.x = crymath::floor(mousePos.x + 0.5f);
	mousePos.y = crymath::floor(mousePos.y + 0.5f);
	mousePos.z = crymath::floor(mousePos.z + 0.5f);
	m_pBuildingEntity->SetPos(mousePos);
}

IEntity* BaseBuildingComponent::AssignBuilding(std::shared_ptr<IEntityComponent> ptr)
{
	if (m_pBuildingEntity) {
		EntityUtils::RemoveEntity(m_pBuildingEntity);
		m_pBuildingEntity = nullptr;
	}
	IEntity* owner = m_pEntity->GetComponent<OwnerInfoComponent>()->GetOwner();
	if (!owner) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "BaseBuildingComponent : (AssignBuilding) Player owner is null !");
		return nullptr;
	}

	Vec3 position = m_pEntity->GetWorldPos();
	m_pBuildingEntity = EntityUtils::SpawnEntity(position, IDENTITY, m_pOwnerInfoComponent->GetOwner());
	//m_pBuildingEntity->AddComponent(ptr);
	return m_pBuildingEntity;
}

void BaseBuildingComponent::CancelAssignedBuilding()
{
	if (!HasBuildingAssigned()) {
		return;
	}
	ResourceManagerComponent* resourceManager = m_pEntity->GetComponent<ResourceManagerComponent>();
	if (!resourceManager) {
		return;
	}
	resourceManager->RefundResources(m_pBuildingEntity->GetComponent<CostComponent>()->GetCost());
	EntityUtils::RemoveEntity(m_pBuildingEntity);
	m_pBuildingEntity = nullptr;
}

void BaseBuildingComponent::CancelBuilding()
{
	if (!HasBuildingAssigned()) {
		return;
	}
	ResourceManagerComponent* resourceManager = m_pEntity->GetComponent<ResourceManagerComponent>();
	if (!resourceManager) {
		return;
	}
	EntityUtils::RemoveEntity(m_pBuildingEntity);
	m_pBuildingEntity = nullptr;
}

IEntity* BaseBuildingComponent::PlaceBuilding(Vec3 at)
{
	if (!m_pBuildingEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "BaseBuildingComponent : (PlaceBuilding) m_pBuildingEntity is null !");
		return nullptr;
	}
	BuildingComponent* building = m_pBuildingEntity->GetComponent<BuildingComponent>();
	if (!building) {
		return nullptr;
	}
	if (!building->CanBePlaced()) {
		return nullptr;
	}

	m_pAudioComponent->ExecuteTrigger(m_buildingPlacementSound);

	m_pBuildingEntity->GetComponent<BuildingComponent>()->Place(m_pBuildingEntity->GetWorldPos());
	IEntity* buildingTemp = m_pBuildingEntity;

	//NavigationAgentTypeID agentTypeId = NavigationAgentTypeID::TNavigationID(1);
	//NavigationMeshID navMeshId = gEnv->pAISystem->GetNavigationSystem()->FindEnclosingMeshID(agentTypeId, m_pBuildingEntity->GetWorldPos());
	//gEnv->pAISystem->GetNavigationSystem()->GetUpdateManager()->RequestGlobalUpdate();

	m_pBuildingEntity = nullptr;
	return buildingTemp;
}

bool BaseBuildingComponent::HasBuildingAssigned()
{
	return m_pBuildingEntity != nullptr;
}

void BaseBuildingComponent::RotateBuildingToLeft()
{
	if (!HasBuildingAssigned()) {
		return;
	}

	Quat rotation = Quat::CreateRotationZ(m_pBuildingEntity->GetRotation().GetRotZ() - 0.01f);
	m_pBuildingEntity->SetRotation(rotation);
}

void BaseBuildingComponent::RotateBuildingToRight()
{
	if (!HasBuildingAssigned()) {
		return;
	}

	Quat rotation = Quat::CreateRotationZ(m_pBuildingEntity->GetRotation().GetRotZ() + 0.01f);
	m_pBuildingEntity->SetRotation(-rotation);
}
