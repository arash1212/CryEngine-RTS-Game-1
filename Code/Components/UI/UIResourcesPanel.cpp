#include "StdAfx.h"
#include "UIResourcesPanel.h"
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
	static void RegisterUIResourcesPanelComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(UIResourcesPanelComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterUIResourcesPanelComponent);
}

void UIResourcesPanelComponent::Initialize()
{
	//Initialize SelectionBoxUIElement and set it visible
	m_pResourcesPanelUIElement = gEnv->pFlashUI->GetUIElement("resource-panel");
	if (m_pResourcesPanelUIElement) {
		m_pResourcesPanelUIElement->SetVisible(true);
	}
	else {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIResourcesPanelComponent : (Initialize) m_pResourcesPanelUIElement is null !");
	}
}

Cry::Entity::EventFlags UIResourcesPanelComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void UIResourcesPanelComponent::ProcessEvent(const SEntityEvent& event)
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

void UIResourcesPanelComponent::SetMoneyAmount(int32 moneyAmount)
{
	if (!m_pResourcesPanelUIElement) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIResourcesPanelComponent : (SetMoneyAmount) ResourcesPanelUIElement is null !");
		return;
	}

	SUIArguments args;
	args.AddArgument(moneyAmount);

	m_pResourcesPanelUIElement->CallFunction("SetMoneyAmount", args);
}

void UIResourcesPanelComponent::SetOilAmount(int32 oilAmount)
{
	if (!m_pResourcesPanelUIElement) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIResourcesPanelComponent : (SetOilAmount) ResourcesPanelUIElement is null !");
		return;
	}

	SUIArguments args;
	args.AddArgument(oilAmount);

	m_pResourcesPanelUIElement->CallFunction("SetOilAmount", args);
}

void UIResourcesPanelComponent::SetPopulationAmount(int32 populationAmount) {

	if (!m_pResourcesPanelUIElement) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIResourcesPanelComponent : (SetPopulationAmount) SetPopulationAmount is null !");
		return;
	}

	SUIArguments args;
	args.AddArgument(populationAmount);

	m_pResourcesPanelUIElement->CallFunction("SetPopulationAmount", args);
}

void UIResourcesPanelComponent::UpdatePanel()
{
	ResourceManagerComponent* resourceManager = m_pEntity->GetComponent<ResourceManagerComponent>();
	if (!resourceManager) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIResourcesPanelComponent : (UpdatePanel) ResourceManagerComponent on entity is null !");
		return;
	}
	SResourceInfo resourceInfo = resourceManager->GetAvailableResourcesInfo();

	//
	SetMoneyAmount(resourceInfo.m_moneyAmount);
	SetOilAmount(resourceInfo.m_oilAmount);
	SetPopulationAmount(resourceInfo.m_populationAmount);

	CryLog("resources panel updated");
}

void UIResourcesPanelComponent::SetEventListener(IUIElementEventListener* eventListener)
{
	this->m_pResourcesPanelUIElement->AddEventListener(eventListener, "resources-panel-event-listener");
}