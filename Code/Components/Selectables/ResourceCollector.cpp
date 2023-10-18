#include "StdAfx.h"
#include "ResourceCollector.h"
#include "GamePlugin.h"

#include <UIItems/IBaseUIItem.h>
#include <UIItems/Items/UICancelItem.h>
#include <UIItems/Items/UIChangeStanceItem.h>
#include <UIItems/Items/Buildings/UIHQ1BuildItem.h>
#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/ResourceManager.h>

#include <Components/Info/OwnerInfo.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterResourceCollectorComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(ResourceCollectorComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterResourceCollectorComponent);
}

void ResourceCollectorComponent::Initialize()
{
	//OwnerComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetOrCreateComponent<OwnerInfoComponent>();
}

Cry::Entity::EventFlags ResourceCollectorComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void ResourceCollectorComponent::ProcessEvent(const SEntityEvent& event)
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

void ResourceCollectorComponent::AddResource(int32 amount)
{
	m_amountResourceCollected += amount;
}

int32 ResourceCollectorComponent::GetAmountOfResourceCollected()
{
	return m_amountResourceCollected;
}

void ResourceCollectorComponent::SendResourceToWareHouse()
{
	if (!m_pOwnerInfoComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceCollectorComponent: (SendResourceToWareHouse) OwnerInfoComponent cannot be null.");
		return;
	}
	IEntity* owner = m_pOwnerInfoComponent->GetOwner();
	if (!owner) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceCollectorComponent: (SendResourceToWareHouse) owner cannot be null.");
		return;
	}
	ResourceManagerComponent* resourceManager = owner->GetComponent<ResourceManagerComponent>();
	if (!resourceManager) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceCollectorComponent: (SendResourceToWareHouse) resourceManager cannot be null.");
		return;
	}

	switch (m_pCurrentResourceType)
	{
	case EResourceType::Money: {
		resourceManager->AddResource(EResourceType::Money, m_amountResourceCollected);
	}break;
	case EResourceType::OIL: {
		resourceManager->AddResource(EResourceType::OIL, m_amountResourceCollected);
	}break;
	default:
		break;
	} m_pCurrentResourceType;

	this->m_amountResourceCollected = 0;
}

int32 ResourceCollectorComponent::GetMaxResourceCanBeCollected()
{
	return m_maxResouceCanBeCollected;
}

void ResourceCollectorComponent::SetCurrentResourceType(EResourceType resourceType)
{
	m_pCurrentResourceType = resourceType;
}

EResourceType ResourceCollectorComponent::GetCurrentResourceType()
{
	return m_pCurrentResourceType;
}

bool ResourceCollectorComponent::CanCollectResource()
{
	return m_amountResourceCollected < m_maxResouceCanBeCollected;
}

void ResourceCollectorComponent::EmptyResources()
{
	m_amountResourceCollected = 0;
}
