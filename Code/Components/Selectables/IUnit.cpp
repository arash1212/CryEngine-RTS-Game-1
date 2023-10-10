#include "StdAfx.h"
#include "IUnit.h"
#include "GamePlugin.h"

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


namespace
{
	static void RegisterIUnitComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(IUnitComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterIUnitComponent);
}



void IUnitComponent::Initialize()
{

}


Cry::Entity::EventFlags IUnitComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void IUnitComponent::ProcessEvent(const SEntityEvent& event)
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

void IUnitComponent::UpdateAnimations()
{
}

void IUnitComponent::Attack(IEntity* target)
{
}

void IUnitComponent::LookAt(Vec3 position)
{
}

void IUnitComponent::AttackRandomTarget()
{
}

void IUnitComponent::Stop()
{
}

void IUnitComponent::MoveTo(Vec3 position, bool run)
{
}

void IUnitComponent::StopMoving()
{
}

void IUnitComponent::FindRandomTarget()
{
}

void IUnitComponent::SetTargetEntity(IEntity* target)
{
}

SUnitAttackInfo IUnitComponent::GetAttackInfo()
{
	return SUnitAttackInfo();
}

f32 IUnitComponent::GetCurrentSpeed()
{
	return f32();
}

bool IUnitComponent::IsRunning()
{
	return false;
}
