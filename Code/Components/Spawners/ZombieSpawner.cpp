#include "StdAfx.h"
#include "ZombieSpawner.h"
#include "GamePlugin.h"

#include <Components/Info/OwnerInfo.h>
#include <Components/Selectables/Units/Zombie1Unit.h>
#include <Components/Managers/ResourceManager.h>

#include <Utils/EntityUtils.h>

#include <CryEntitySystem/IEntitySystem.h>
#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterZombieSpawnerComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(ZombieSpawnerComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterZombieSpawnerComponent);
}

void ZombieSpawnerComponent::Initialize()
{
	//OwnerComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetOrCreateComponent<OwnerInfoComponent>();
	m_pOwnerInfoComponent->SetCanBeTarget(false);
	m_pOwnerInfoComponent->SetPlayer(EPlayer::FERAL);
	m_pOwnerInfoComponent->SetTeam(EPlayerTeam::FERAL);
	m_pOwnerInfoComponent->SetFaction(EPlayerFaction::FERAL);

	//ResourceManagerComponent initialization
	m_pResourceManagerComponent = m_pEntity->GetOrCreateComponent<ResourceManagerComponent>();
	m_pResourceManagerComponent->SetIsPlayer(false);
}

Cry::Entity::EventFlags ZombieSpawnerComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void ZombieSpawnerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {
		bIsGameStarted = true;

	}break;
	case Cry::Entity::EEvent::Update: {
		f32 DeltaTime = event.fParam[0];

		SpawnZombies();

		if (m_spawnTimePassed < m_timeBetweenSpawningZombies) {
			m_spawnTimePassed += 0.5f * DeltaTime;
		}
	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

void ZombieSpawnerComponent::SpawnZombies()
{
	if (!bIsGameStarted) {
		return;
	}
	if (m_pResourceManagerComponent->GetOwnedEntities().size() >= m_maxZombiesCount) {
		return;
	}

	if (m_spawnTimePassed >= m_timeBetweenSpawningZombies) {
		Vec3 position = m_pEntity->GetWorldPos();
		Quat rotation = IDENTITY;
		IEntity* spawnedEntity = EntityUtils::SpawnEntity(position, rotation, m_pEntity);
		if (!spawnedEntity) {
			return;
		}
		spawnedEntity->GetOrCreateComponent<Zombie1UnitComponent>();
		m_spawnTimePassed = 0;
	}
}
