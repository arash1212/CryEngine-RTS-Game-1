#include "StdAfx.h"
#include "Engineer.h"
#include "GamePlugin.h"

#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/UnitStateManager.h>
#include <Components/Controller/AIController.h>
#include <Components/Weapons/BaseWeapon.h>
#include <Components/Selectables/UnitAnimation.h>
#include <Components/Managers/ActionManager.h>

#include <Utils/MathUtils.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>


namespace
{
	static void RegisterEngineerComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(EngineerComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterEngineerComponent);
}


void EngineerComponent::Initialize()
{

}


Cry::Entity::EventFlags EngineerComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void EngineerComponent::ProcessEvent(const SEntityEvent& event)
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

void EngineerComponent::AssignBuilding(IEntity* buildingEntity)
{
	this->m_pBuildingAssigned = buildingEntity;
}

bool EngineerComponent::HasBuildingAssigned()
{
	return this->m_pBuildingAssigned;
}

void EngineerComponent::CancelBuildingAssigned()
{
	this->m_pBuildingAssigned = nullptr;
}

void EngineerComponent::SetEngineerInfo(SEngineerInfo engineerInfo)
{
	this->m_pEngineerInfo = engineerInfo;
}

SEngineerInfo EngineerComponent::GetEngineerInfo()
{
	return m_pEngineerInfo;
}
