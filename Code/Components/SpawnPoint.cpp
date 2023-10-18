// Copyright 2017-2019 Crytek GmbH / Crytek Group. All rights reserved.
#include "StdAfx.h"
#include "SpawnPoint.h"
#include "Player/Player.h"

#include <Components/Selectables/Units/Soldier1Unit.h>
#include <Components/BaseBuilding/Building.h>
#include <Components/BaseBuilding/Buildings/HQ1Building.h>
#include <Components/Info/OwnerInfo.h>

#include <CrySchematyc/Reflection/TypeDesc.h>
#include <CrySchematyc/Utils/EnumFlags.h>
#include <CrySchematyc/Env/IEnvRegistry.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Env/Elements/EnvSignal.h>
#include <CrySchematyc/ResourceTypes.h>
#include <CrySchematyc/MathTypes.h>
#include <CrySchematyc/Utils/SharedString.h>
#include <CryCore/StaticInstanceList.h>

static void RegisterSpawnPointComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(SpawnPointComponent));
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterSpawnPointComponent)

void SpawnPointComponent::Initialize()
{
}

Cry::Entity::EventFlags SpawnPointComponent::GetEventMask() const
{
	return 
		Cry::Entity::EEvent::GameplayStarted|
		Cry::Entity::EEvent::Update | 
		Cry::Entity::EEvent::Reset;
}

void SpawnPointComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {
		SpawnPlayer();

	}break;
	case Cry::Entity::EEvent::Update: {

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

void SpawnPointComponent::SpawnPlayer()
{
	SEntitySpawnParams pPlayerSpawnParams;
	pPlayerSpawnParams.vPosition = m_pEntity->GetWorldPos();
	m_pPlayerEntity = gEnv->pEntitySystem->SpawnEntity(pPlayerSpawnParams);

	m_pPlayerEntity->GetOrCreateComponent<PlayerComponent>();
	m_pPlayerEntity->GetComponent<OwnerInfoComponent>()->SetOwner(m_pPlayerEntity);

	SpawnPlayerHQBuilding(m_pPlayerEntity);
}

void SpawnPointComponent::SpawnPlayerHQBuilding(IEntity* owner)
{
	SEntitySpawnParams pHQBuildingSpawnParams;
	pHQBuildingSpawnParams.vPosition = m_pEntity->GetWorldPos();
	pHqBuildingEntity = gEnv->pEntitySystem->SpawnEntity(pHQBuildingSpawnParams);

	pHqBuildingEntity->GetOrCreateComponent<HQ1BuildingComponent>();
	pHqBuildingEntity->GetComponent<OwnerInfoComponent>()->SetOwner(owner);
	pHqBuildingEntity->GetComponent<BuildingComponent>()->SetBuilt();
}
