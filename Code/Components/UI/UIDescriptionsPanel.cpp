#include "StdAfx.h"
#include "UIDescriptionsPanel.h"
#include "GamePlugin.h"

#include <FlashUI/FlashUIElement.h>
#include <CryEntitySystem/IEntitySystem.h>

#include <Components/UI/Listener/UIElementEventListener.h>
#include <UIItems/DescriptionPanel/BaseDescriptionPanelItem.h>


#include <CryRenderer/IRenderer.h>
#include <CryEntitySystem/IEntitySystem.h>
#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterUIDescriptionsPanelComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(UIDescriptionsPanelComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterUIDescriptionsPanelComponent);
}

void UIDescriptionsPanelComponent::Initialize()
{
	//Initialize InfoPanelUIElement and set it visible
	m_pDescriptionPanelUIElement = gEnv->pFlashUI->GetUIElement("description-panel");
	if (m_pDescriptionPanelUIElement) {
		m_pDescriptionPanelUIElement->SetVisible(true);
	}
	else {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIDescriptionsPanelComponent : (Initialize) m_pDescriptionPanelUIElement is null !");
	}
}

Cry::Entity::EventFlags UIDescriptionsPanelComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void UIDescriptionsPanelComponent::ProcessEvent(const SEntityEvent& event)
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

void UIDescriptionsPanelComponent::AddItem(BaseDescriptionPanelItem* item)
{
	if (!item) {
		return;
	}

	if (!m_pDescriptionPanelUIElement) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIDescriptionsPanelComponent : (AddItem) DescriptionPanelUIElement is null !");
		return;
	}

	SUIArguments args;
	args.AddArgument(item->GetImagePath());
	args.AddArgument(item->GetAmount());
	args.AddArgument("");

	m_pDescriptionPanelUIElement->CallFunction("addItem", args);
	m_items.append(item);
}

void UIDescriptionsPanelComponent::Clear()
{
	if (!m_pDescriptionPanelUIElement) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIDescriptionsPanelComponent : (Clear) DescriptionPanelUIElement is null !");
		return;
	}

	m_pDescriptionPanelUIElement->CallFunction("clear");
	m_items.clear();
}

void UIDescriptionsPanelComponent::SetAmount(int32 index, int32 amount)
{
	if (!m_pDescriptionPanelUIElement) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIInfoPanelComponent : (SetAmount) DescriptionPanelUIElement is null !");
		return;
	}

	SUIArguments args;
	args.AddArgument(index);
	args.AddArgument(amount);
	m_pDescriptionPanelUIElement->CallFunction("SetCount", args);
}

void UIDescriptionsPanelComponent::SetDescriptionText(string description)
{
	if (!m_pDescriptionPanelUIElement) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIInfoPanelComponent : (SetDescriptionText) DescriptionPanelUIElement is null !");
		return;
	}

	SUIArguments args;
	args.AddArgument(description);
	m_pDescriptionPanelUIElement->CallFunction("SetDescriptionText", args);
}

void UIDescriptionsPanelComponent::SetEventListener(IUIElementEventListener* eventListener)
{
	this->m_pDescriptionPanelUIElement->AddEventListener(eventListener, "description-panel-event-listener");
}
