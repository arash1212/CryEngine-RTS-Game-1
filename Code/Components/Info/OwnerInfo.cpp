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

EPlayerTeam OwnerInfoComponent::GetTeam()
{
	return this->m_pOwnerInfo.m_pTeam;
}

void OwnerInfoComponent::SetFaction(EPlayerFaction faction)
{
	this->m_pOwnerInfo.m_pFaction = faction;
}

bool OwnerInfoComponent::IsEntityHostile(IEntity* entity)
{
	OwnerInfoComponent* otherOwner = entity->GetComponent<OwnerInfoComponent>();
	if (!otherOwner) {
		return false;
	}
	return otherOwner->GetInfo().m_pTeam != this->m_pOwnerInfo.m_pTeam && bCanBeTarget;
}

bool OwnerInfoComponent::CanBeTarget()
{
	return bCanBeTarget;
}

void OwnerInfoComponent::SetCanBeTarget(bool canBeTarget)
{
	this->bCanBeTarget = canBeTarget;
}

void OwnerInfoComponent::SetOwner(IEntity* owner)
{
	OwnerInfoComponent* ownerInfo = owner->GetComponent<OwnerInfoComponent>();
	if (!ownerInfo) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "OwnerInfoComponent : (SetOwner) pOwnerInfo is null !");
		return;
	}
	this->m_pOwner = owner;
	this->m_pOwnerInfo.m_pPlayer = ownerInfo->GetInfo().m_pPlayer;
	this->m_pOwnerInfo.m_pTeam = ownerInfo->GetInfo().m_pTeam;
	this->m_pOwnerInfo.m_pFaction = ownerInfo->GetInfo().m_pFaction;
}

IEntity* OwnerInfoComponent::GetOwner()
{
	return m_pOwner;
}

EPlayer OwnerInfoComponent::GetPlayer()
{
	return m_pOwnerInfo.m_pPlayer;
}

void OwnerInfoComponent::SetPlayer(EPlayer player)
{
	this->m_pOwnerInfo.m_pPlayer = player;
}
