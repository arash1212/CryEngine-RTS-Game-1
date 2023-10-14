#include "StdAfx.h"
#include "Actionbar.h"
#include "GamePlugin.h"

#include <FlashUI/FlashUIElement.h>
#include <CryEntitySystem/IEntitySystem.h>

#include <Components/UI/Listener/UIElementEventListener.h>

#include <CryRenderer/IRenderer.h>
#include <CryEntitySystem/IEntitySystem.h>
#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterActionbarComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(ActionbarComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterActionbarComponent);
}

void ActionbarComponent::Initialize()
{
	//Initialize ActionbarUIElement and set it visible
	m_pActionbarUIElement = gEnv->pFlashUI->GetUIElement("action-bar");
	if (m_pActionbarUIElement) {
		m_pActionbarUIElement->SetVisible(true);
	}
	else {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ActionbarComponent : (Initialize) m_pActionbarUIElement is null !");
	}
}

Cry::Entity::EventFlags ActionbarComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void ActionbarComponent::ProcessEvent(const SEntityEvent& event)
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

void ActionbarComponent::AddButton(string imagePath)
{
	if (!m_pActionbarUIElement) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ActionbarComponent : (AddButton) ActionbarUIElement is null !");
		return;
	}

	SUIArguments args;
	args.AddArgument(imagePath);

	m_pActionbarUIElement->CallFunction("addButton", args);
}

void ActionbarComponent::Clear()
{
	if (!m_pActionbarUIElement) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ActionbarComponent : (Clear) ActionbarUIElement is null !");
		return;
	}

	m_pActionbarUIElement->CallFunction("clear");
}

void ActionbarComponent::SetEventListener(IUIElementEventListener* eventListener)
{
	this->m_pActionbarUIElement->AddEventListener(eventListener, "actionbar-event-listener");
}
