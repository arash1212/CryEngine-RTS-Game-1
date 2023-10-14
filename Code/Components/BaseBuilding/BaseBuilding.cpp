#include "StdAfx.h"
#include "BaseBuilding.h"
#include "GamePlugin.h"

#include <CryEntitySystem/IEntitySystem.h>

#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Controller/AIController.h>
#include <Components/Weapons/BaseWeapon.h>
#include <Components/Selectables/UnitAnimation.h>
#include <Components/Managers/ActionManager.h>

#include <Utils/MathUtils.h>
#include <Utils/MouseUtils.h>

#include <Components/BaseBuilding/Building.h>
#include <Components\BaseBuilding\Buildings\HQ1Building.h>

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
	m_pBuildingEntity->SetPos(mousePos);
}

IEntity* BaseBuildingComponent::AssignBuilding()
{
	if (m_pBuildingEntity) {
		gEnv->pEntitySystem->RemoveEntity(m_pBuildingEntity->GetId());
		m_pBuildingEntity = nullptr;
	}

	SEntitySpawnParams buildingEntitySpawnParams;
	buildingEntitySpawnParams.vPosition = m_pEntity->GetWorldPos();
	//buildingEntitySpawnParams.qRotation = ;
	m_pBuildingEntity = gEnv->pEntitySystem->SpawnEntity(buildingEntitySpawnParams, true);
	return m_pBuildingEntity;
}

void BaseBuildingComponent::CancelAssignedBuilding()
{
	if (!HasBuildingAssigned()) {
		return;
	}
	if (!m_pBuildingEntity) {
		return;
	}
	gEnv->pEntitySystem->RemoveEntity(m_pBuildingEntity->GetId());
	m_pBuildingEntity = nullptr;
}

void BaseBuildingComponent::PlaceBuilding(Vec3 at)
{
	if (!m_pBuildingEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "BaseBuildingComponent : (PlaceBuilding) m_pBuildingEntity is null .!");
		return;
	}
	BuildingComponent* building = m_pBuildingEntity->GetComponent<BuildingComponent>();
	if (!building) {
		return;
	}
	if (!building->CanBePlaced()) {
		return;
	}

	m_pAudioComponent->ExecuteTrigger(m_buildingPlacementSound);

	m_pBuildingEntity->GetComponent<BuildingComponent>()->Place(at);
	m_pBuildingEntity = nullptr;
}

bool BaseBuildingComponent::HasBuildingAssigned()
{
	return m_pBuildingEntity != nullptr;
}
