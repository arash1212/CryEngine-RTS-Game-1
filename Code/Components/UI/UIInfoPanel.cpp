#include "StdAfx.h"
#include "UIInfoPanel.h"
#include "GamePlugin.h"

#include <FlashUI/FlashUIElement.h>
#include <CryEntitySystem/IEntitySystem.h>

#include <Components/UI/Listener/UIElementEventListener.h>
#include <UIItems/InfoPanel/IBaseInfoPanelUIItem.h>

#include <CryRenderer/IRenderer.h>
#include <CryEntitySystem/IEntitySystem.h>
#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterUIInfoPanelComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(UIInfoPanelComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterUIInfoPanelComponent);
}

void UIInfoPanelComponent::Initialize()
{
	//Initialize InfoPanelUIElement and set it visible
	m_pInfoPanelUIElement = gEnv->pFlashUI->GetUIElement("info-panel");
	if (m_pInfoPanelUIElement) {
		m_pInfoPanelUIElement->SetVisible(true);
	}
	else {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIInfoPanelComponent : (Initialize) m_pInfoPanelUIElement is null !");
	}
}

Cry::Entity::EventFlags UIInfoPanelComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void UIInfoPanelComponent::ProcessEvent(const SEntityEvent& event)
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

void UIInfoPanelComponent::AddItem(IBaseInfoPanelUIItem* item)
{
	if (!item) {
		return;
	}

	if (!m_pInfoPanelUIElement) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIInfoPanelComponent : (AddItem) InfoPanelUIElement is null !");
		return;
	}

	SUIArguments args;
	args.AddArgument(item->GetImagePath());

	m_pInfoPanelUIElement->CallFunction("addItem", args);
	m_items.append(item);
}

void UIInfoPanelComponent::Clear()
{
	if (!m_pInfoPanelUIElement) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIInfoPanelComponent : (Clear) InfoPanelUIElement is null !");
		return;
	}

	m_pInfoPanelUIElement->CallFunction("clear");
	m_items.clear();
}

void UIInfoPanelComponent::SetCount(int32 index, int32 count)
{
	if (!m_pInfoPanelUIElement) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIInfoPanelComponent : (SetCount) InfoPanelUIElement is null !");
		return;
	}

	SUIArguments args;
	args.AddArgument(index);
	args.AddArgument(count);

	m_pInfoPanelUIElement->CallFunction("SetCount", args);
}

void UIInfoPanelComponent::SetMainIcon(string imagePath, string text)
{
	if (!m_pInfoPanelUIElement) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIInfoPanelComponent : (AddItem) InfoPanelUIElement is null !");
		return;
	}

	SUIArguments args;
	args.AddArgument(imagePath);
	args.AddArgument(text);

	m_pInfoPanelUIElement->CallFunction("SetMainIcon", args);
}

void UIInfoPanelComponent::ExecuteItem(int32 index)
{
	m_items[index]->Execute();
}

void UIInfoPanelComponent::SetEventListener(IUIElementEventListener* eventListener)
{
	this->m_pInfoPanelUIElement->AddEventListener(eventListener, "info-panel-event-listener");
}
