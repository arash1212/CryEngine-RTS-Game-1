#include "StdAfx.h"
#include "Health.h"
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
	static void RegisterHealthComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(HealthComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterHealthComponent);
}

void HealthComponent::Initialize()
{
}

Cry::Entity::EventFlags HealthComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void HealthComponent::ProcessEvent(const SEntityEvent& event)
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

void HealthComponent::ApplyDamage(f32 damage)
{
	this->m_currentHealth -= damage;

	if (m_currentHealth <= 0) {
		Die();
	}
}

void HealthComponent::Die()
{
	EntityUtils::RemoveEntity(m_pEntity);
}

void HealthComponent::SetMaxHealth(f32 maxHealth)
{
	this->m_maxHealth = maxHealth;
	this->m_currentHealth = maxHealth;
}

f32 HealthComponent::GetCurrentHealth()
{
	return m_currentHealth;
}
