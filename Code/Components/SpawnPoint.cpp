// Copyright 2017-2019 Crytek GmbH / Crytek Group. All rights reserved.
#include "StdAfx.h"
#include "SpawnPoint.h"
#include "Player/Player.h"

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
	IEntity* pPlayerEntity = gEnv->pEntitySystem->SpawnEntity(pPlayerSpawnParams);

	pPlayerEntity->GetOrCreateComponent<PlayerComponent>();
}
