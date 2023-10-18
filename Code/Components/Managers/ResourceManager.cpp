#include "StdAfx.h"
#include "ResourceManager.h"
#include "GamePlugin.h"

#include <Actions/IBaseAction.h>
#include <Components/UI/UIResourcesPanel.h>
#include <Components/Resources/Resource.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterResourceManagerComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(ResourceManagerComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterResourceManagerComponent);
}

void ResourceManagerComponent::Initialize()
{
	m_pResouecesPanelComponent = m_pEntity->GetComponent<UIResourcesPanelComponent>();
}

Cry::Entity::EventFlags ResourceManagerComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void ResourceManagerComponent::ProcessEvent(const SEntityEvent& event)
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

SResourceInfo ResourceManagerComponent::GetAvailableResourcesInfo()
{
	return m_pResouceInfo;
}

bool ResourceManagerComponent::RequsetResources(SResourceInfo resourceRequestParams)
{

	if (m_pResouceInfo.m_moneyAmount < resourceRequestParams.m_moneyAmount) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceManagerComponent : (RequsetResources) Not Enough Money");
		return false;
	}
	else if (m_pResouceInfo.m_oilAmount < resourceRequestParams.m_oilAmount) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceManagerComponent : (RequsetResources) Not Enough Oil");
		return false;
	}

	m_pResouceInfo.m_moneyAmount -= resourceRequestParams.m_moneyAmount;
	m_pResouceInfo.m_oilAmount -= resourceRequestParams.m_oilAmount;
	m_pResouecesPanelComponent->UpdatePanel();
	return true;
}

void ResourceManagerComponent::RefundResources(SResourceInfo resourceRequestParams)
{
	m_pResouceInfo.m_moneyAmount += resourceRequestParams.m_moneyAmount;
	m_pResouceInfo.m_oilAmount += resourceRequestParams.m_oilAmount;
	m_pResouecesPanelComponent->UpdatePanel();
	CryLog("resource refunded");
}

void ResourceManagerComponent::AddResource(EResourceType type, int32 amount)
{
	switch (type)
	{
	case EResourceType::Money: {
		m_pResouceInfo.m_moneyAmount += amount;
	}break;
	case EResourceType::OIL: {
		m_pResouceInfo.m_oilAmount += amount;
	}break;
	default:
		break;
	}
	m_pResouecesPanelComponent->UpdatePanel();
}
