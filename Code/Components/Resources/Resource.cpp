#include "StdAfx.h"
#include "Resource.h"
#include "GamePlugin.h"

#include <UIItems/IBaseUIItem.h>
#include <UIItems/Items/UICancelItem.h>
#include <UIItems/Items/UIChangeStanceItem.h>
#include <UIItems/Items/Buildings/UIHQ1BuildItem.h>
#include <Components/Selectables/Selectable.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterResourceComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(ResourceComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterResourceComponent);
}

void ResourceComponent::Initialize()
{
	//AnimationComponent Initialization
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();

	//SelectableComponent Initialization
	m_pSelectableComponent = m_pEntity->GetOrCreateComponent<SelectableComponent>();
	m_pSelectableComponent->SetIsBuilding(true);

	//CollectingLocationAttachment Initialization
	m_pCollectingLocationAttachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("collectingLocation");
}

Cry::Entity::EventFlags ResourceComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void ResourceComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];

		IPersistantDebug* pd = gEnv->pGameFramework->GetIPersistantDebug();
		if (pd) {
			pd->Begin("testeststs", true);
			pd->AddSphere(GetCollectingLocation(), 0.6f, ColorF(1, 0, 0), 0.6f);
		}
	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

Vec3 ResourceComponent::GetCollectingLocation()
{
	return m_pCollectingLocationAttachment->GetAttWorldAbsolute().t;
}

void ResourceComponent::SetType(EResourceType type)
{
	this->m_type = type;
}

EResourceType ResourceComponent::GetType()
{
	return m_type;
}

