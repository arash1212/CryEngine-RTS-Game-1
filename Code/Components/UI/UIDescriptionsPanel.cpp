#include "StdAfx.h"
#include "UIDescriptionsPanel.h"
#include "GamePlugin.h"

#include <FlashUI/FlashUIElement.h>
#include <CryEntitySystem/IEntitySystem.h>

#include <Components/UI/Listener/UIElementEventListener.h>
#include <UIItems/DescriptionPanel/BaseDescriptionPanelItem.h>

#include <Resources/Resources/AK47Resource.h>
#include <Resources/Resources/BreadResource.h>
#include <Resources/Resources/BulletResource.h>
#include <Resources/Resources/FlourResource.h>
#include <Resources/Resources/GunPowderResource.h>
#include <Resources/Resources/IronResource.h>
#include <Resources/Resources/MoneyResource.h>
#include <Resources/Resources/OilResource.h>
#include <Resources/Resources/PopulationResource.h>
#include <Resources/Resources/SulfurResource.h>
#include <Resources/Resources/WheatResource.h>
#include <Resources/Resources/WoodResource.h>

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
	//ResourceManagerComponent initialization
	m_pResourceManagerComponent = m_pEntity->GetComponent<ResourceManagerComponent>();

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

		UpdateDescriptions();

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
	args.AddArgument(item->GetResource()->GetBuyIcon());
	args.AddArgument(item->GetText());
	args.AddArgument("");

	TUIData data;
	m_pDescriptionPanelUIElement->CallFunction("addItem", args, &data);

	int32 index = -1;
	data.GetValueWithConversion(index);
	item->SetIndex(index);
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

void UIDescriptionsPanelComponent::SetText(int32 index, string text, bool isRed)
{
	if (!m_pDescriptionPanelUIElement) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIInfoPanelComponent : (SetText) DescriptionPanelUIElement is null !");
		return;
	}

	SUIArguments args;
	args.AddArgument(index);
	args.AddArgument(text);
	args.AddArgument(isRed);
	m_pDescriptionPanelUIElement->CallFunction("SetText", args);
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

void UIDescriptionsPanelComponent::AddDescription(SDescription description)
{
	this->Clear();
	if (description.price.m_moneyAmount > 0) {
		this->AddItem(new BaseDescriptionPanelItem(RESOURCE_MONEY, description.price.m_moneyAmount, m_pResourceManagerComponent));
	}
	if (description.price.m_ironAmount > 0) {
		this->AddItem(new BaseDescriptionPanelItem(RESOURCE_IRON, description.price.m_ironAmount, m_pResourceManagerComponent));
	}
	if (description.price.m_woodAmount > 0) {
		this->AddItem(new BaseDescriptionPanelItem(RESOURCE_WOOD, description.price.m_woodAmount, m_pResourceManagerComponent));
	}
	if (description.price.m_oilAmount > 0) {
		this->AddItem(new BaseDescriptionPanelItem(RESOURCE_OIL, description.price.m_oilAmount, m_pResourceManagerComponent));
	}
	if (description.price.m_breadAmount > 0) {
		this->AddItem(new BaseDescriptionPanelItem(RESOURCE_BREAD, description.price.m_breadAmount, m_pResourceManagerComponent));
	}
	if (description.price.m_wheatAmount > 0) {
		this->AddItem(new BaseDescriptionPanelItem(RESOURCE_WHEAT, description.price.m_wheatAmount, m_pResourceManagerComponent));
	}
	if (description.price.m_ak47Amount > 0) {
		this->AddItem(new BaseDescriptionPanelItem(RESOURCE_AK47, description.price.m_ak47Amount, m_pResourceManagerComponent));
	}
	if (description.price.m_bulletAmount > 0) {
		this->AddItem(new BaseDescriptionPanelItem(RESOURCE_BULLET, description.price.m_bulletAmount, m_pResourceManagerComponent));
	}
	if (description.price.m_flourAmount > 0) {
		this->AddItem(new BaseDescriptionPanelItem(RESOURCE_FLOUR, description.price.m_flourAmount, m_pResourceManagerComponent));
	}
	if (description.price.m_gunPowderAmount > 0) {
		this->AddItem(new BaseDescriptionPanelItem(RESOURCE_GUN_POWDER, description.price.m_gunPowderAmount, m_pResourceManagerComponent));
	}
	if (description.price.m_populationAmount > 0) {
		this->AddItem(new BaseDescriptionPanelItem(RESOURCE_POPULATION, description.price.m_populationAmount, m_pResourceManagerComponent));
	}
	if (description.price.m_sulfurAmount > 0) {
		this->AddItem(new BaseDescriptionPanelItem(RESOURCE_SULFUR, description.price.m_sulfurAmount, m_pResourceManagerComponent));
	}
	this->SetDescriptionText(description.sBuyDescription);
}

void UIDescriptionsPanelComponent::UpdateDescriptions()
{
	if (m_items.size() <= 0) {
		return;
	}
	for (BaseDescriptionPanelItem* item : m_items) {
		item->UpdateText();
		this->SetText(item->GetIndex(), item->GetText(), item->IsRed());
	}
}

void UIDescriptionsPanelComponent::SetEventListener(IUIElementEventListener* eventListener)
{
	this->m_pDescriptionPanelUIElement->AddEventListener(eventListener, "description-panel-event-listener");
}

DynArray<BaseDescriptionPanelItem*> UIDescriptionsPanelComponent::GetItems()
{
	return m_items;
}
