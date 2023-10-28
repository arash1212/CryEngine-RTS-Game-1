#include "StdAfx.h"
#include "UIHealthbars.h"
#include "GamePlugin.h"

#include "FlashUI/FlashUI.h"
#include "FlashUI/FlashUIElement.h"

#include <Components/UI/Listener/UIElementEventListener.h>
#include <Components/Selectables/Selectable.h>
#include <Components/Managers/ResourceManager.h>

#include <CryRenderer/IRenderer.h>
#include <CryEntitySystem/IEntitySystem.h>
#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterUIHealthbarsComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(UIHealthbarsComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterUIHealthbarsComponent);
}

void UIHealthbarsComponent::Initialize()
{
	//Initialize SelectionBoxUIElement and set it visible
	m_pHealthbarUIElement = gEnv->pFlashUI->GetUIElement("health-bar");
	if (m_pHealthbarUIElement) {
		m_pHealthbarUIElement->SetVisible(true);
		CryLog("Healthbarsssssss,,,,");
	}
	else {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIHealthbarsComponent : (Initialize) m_pHealthbarUIElement is null !");
	}
}

Cry::Entity::EventFlags UIHealthbarsComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void UIHealthbarsComponent::ProcessEvent(const SEntityEvent& event)
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

void UIHealthbarsComponent::AddHealthbar(int32 x, int32 y)
{
	if (!m_pHealthbarUIElement) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIHealthbarsComponent : (AddHealthbar) m_pHealthbarUIElement is null !");
		return;
	}

	SUIArguments args;
	args.AddArgument(x);
	args.AddArgument(y);

	m_pHealthbarUIElement->CallFunction("AddHealthbar", args);
}

void UIHealthbarsComponent::SetEventListener(IUIElementEventListener* eventListener)
{
	this->m_pHealthbarUIElement->AddEventListener(eventListener, "actionbar-event-listener");
}