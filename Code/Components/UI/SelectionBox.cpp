#include "StdAfx.h"
#include "SelectionBox.h"
#include "GamePlugin.h"

#include "FlashUI/FlashUI.h"
#include "FlashUI/FlashUIElement.h"

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterSelectionBoxComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(SelectionBoxComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterSelectionBoxComponent);
}

void SelectionBoxComponent::Initialize()
{
	//Initialize SelectionBoxUIElement and set it visible
	m_pSelectionBoxUIElement = gEnv->pFlashUI->GetUIElement("selection-box");
	if (m_pSelectionBoxUIElement) {
		m_pSelectionBoxUIElement->SetVisible(true);
	}
	else {
		CryLog("SelectionBoxComponent : (Initialize) m_pSelectionBoxUIElement is null !");
	}
}

Cry::Entity::EventFlags SelectionBoxComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void SelectionBoxComponent::ProcessEvent(const SEntityEvent& event)
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