#include "StdAfx.h"
#include "InfoPanelUIDetail.h"
#include "GamePlugin.h"

#include <UIItems/IBaseUIItem.h>
#include <UIItems/Items/UICancelItem.h>
#include <UIItems/Items/UIChangeStanceItem.h>
#include <UIItems/Items/Buildings/UIHQ1BuildItem.h>
#include <Components/Managers/ResourceManager.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterInfoPanelUIDetailComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(InfoPanelUIDetailComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterInfoPanelUIDetailComponent);
}

void InfoPanelUIDetailComponent::Initialize()
{

}

Cry::Entity::EventFlags InfoPanelUIDetailComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void InfoPanelUIDetailComponent::ProcessEvent(const SEntityEvent& event)
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

void InfoPanelUIDetailComponent::SetInfoPanelUIItem(IBaseInfoPanelUIItem* infoPanelItem)
{
	this->m_pUIInfoPanelUIItem = infoPanelItem;
}

IBaseInfoPanelUIItem* InfoPanelUIDetailComponent::GetInfoPanelUIItem()
{
	return m_pUIInfoPanelUIItem;
}
