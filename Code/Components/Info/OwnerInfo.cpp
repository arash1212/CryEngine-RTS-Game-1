#include "StdAfx.h"
#include "OwnerInfo.h"
#include "GamePlugin.h"

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterOwnerInfoComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(OwnerInfoComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterOwnerInfoComponent);
}

void OwnerInfoComponent::Initialize()
{
}

Cry::Entity::EventFlags OwnerInfoComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void OwnerInfoComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

SOwnerInfo OwnerInfoComponent::GetInfo()
{
	return m_pOwnerInfo;
}

void OwnerInfoComponent::SetTeam(EPlayerTeam team)
{
	this->m_pOwnerInfo.m_pTeam = team;
}

void OwnerInfoComponent::SetFaction(EPlayerFaction faction)
{
	this->m_pOwnerInfo.m_pFaction = faction;
}
