#include "StdAfx.h"
#include "ResourceManager.h"
#include "GamePlugin.h"

#include <Actions/IBaseAction.h>
#include <Components/UI/UIResourcesPanel.h>
#include <Components/Resources/Resource.h>
#include <Components/BaseBuilding/Building.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterResourceManagerComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(ResourceManagerComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterResourceManagerComponent);
}

void ResourceManagerComponent::Initialize()
{
}

Cry::Entity::EventFlags ResourceManagerComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void ResourceManagerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];

		//If is Player add UI stuff
		if (bIsPlayer && !bIsinitDone) {
			//ResouecesPanelComponent initialization
			m_pResouecesPanelComponent = m_pEntity->GetComponent<UIResourcesPanelComponent>();

			//Set Resources Amount
			m_pResouceInfo.m_moneyAmount = 700;
			m_pResouceInfo.m_oilAmount = 300;
			m_pResouceInfo.m_populationAmount = 20;
			m_pResouceInfo.m_populationUsed = 0;
			m_pResouceInfo.m_wheatAmount = 70;
			m_pResouceInfo.m_flourAmount = 50;
			m_pResouceInfo.m_woodAmount = 500;
			m_pResouceInfo.m_breadAmount = 30;

			//AudioComponent initialization
			m_pAudioComponent = m_pEntity->GetComponent<IEntityAudioComponent>();

			//Sounds
			m_pBuySound = CryAudio::StringToId("buy_sound_1");
			m_pSellSound = CryAudio::StringToId("sell_sound_1");

			m_pResouecesPanelComponent->UpdatePanel();

			bIsinitDone = true;
		}

		if (bIsPlayer && bIsinitDone) {
			UpdatePopulation();
		}

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

void ResourceManagerComponent::UpdatePopulation()
{
	int32 currentPopulation = 0;
	for (IEntity* entity : m_pOwnedEntities) {
		if (!entity) {
			continue;
		}
		BuildingComponent* pBuildingComponet = entity->GetComponent<BuildingComponent>();
		if (!pBuildingComponet) {
			continue;
		}
		else if(pBuildingComponet->IsHouse() && pBuildingComponet->IsBuilt()){
			currentPopulation += pBuildingComponet->GetBuildingInfos().m_populationProduces;
		}
	}
	currentPopulation -= m_pResouceInfo.m_populationUsed;
	m_pResouceInfo.m_populationAmount = currentPopulation;
	m_pResouecesPanelComponent->SetPopulationAmount(currentPopulation);
}

SResourceInfo ResourceManagerComponent::GetAvailableResourcesInfo()
{
	return m_pResouceInfo;
}

bool ResourceManagerComponent::RequsetResources(SResourceInfo resourceRequestParams)
{

	if (m_pResouceInfo.m_moneyAmount < resourceRequestParams.m_moneyAmount) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceManagerComponent : (RequsetResources) Not Enough Money");
		return false;
	}
	else if (m_pResouceInfo.m_oilAmount < resourceRequestParams.m_oilAmount) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceManagerComponent : (RequsetResources) Not Enough Oil");
		return false;
	}
	else if (m_pResouceInfo.m_populationAmount < resourceRequestParams.m_populationAmount) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceManagerComponent : (RequsetResources) Not Enough Workers");
		return false;
	}
	else if (m_pResouceInfo.m_wheatAmount < resourceRequestParams.m_wheatAmount) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceManagerComponent : (RequsetResources) Not Enough Wheat");
		return false;
	}
	else if (m_pResouceInfo.m_flourAmount < resourceRequestParams.m_flourAmount) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceManagerComponent : (RequsetResources) Not Enough Flour");
		return false;
	}
	else if (m_pResouceInfo.m_woodAmount < resourceRequestParams.m_woodAmount) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceManagerComponent : (RequsetResources) Not Enough Wood");
		return false;
	}
	else if (m_pResouceInfo.m_breadAmount < resourceRequestParams.m_breadAmount) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceManagerComponent : (RequsetResources) Not Enough Bread");
		return false;
	}

	m_pResouceInfo.m_moneyAmount -= resourceRequestParams.m_moneyAmount;
	m_pResouceInfo.m_oilAmount -= resourceRequestParams.m_oilAmount;
	m_pResouceInfo.m_populationUsed += resourceRequestParams.m_populationAmount;
	m_pResouceInfo.m_wheatAmount -= resourceRequestParams.m_wheatAmount;
	m_pResouceInfo.m_flourAmount -= resourceRequestParams.m_flourAmount;
	m_pResouceInfo.m_woodAmount -= resourceRequestParams.m_woodAmount;
	m_pResouceInfo.m_breadAmount -= resourceRequestParams.m_breadAmount;

	m_pResouecesPanelComponent->UpdatePanel();
	return true;
}

bool ResourceManagerComponent::CheckIfResourcesAvailable(SResourceInfo resourceRequestParams)
{

	if (m_pResouceInfo.m_moneyAmount < resourceRequestParams.m_moneyAmount) {
		return false;
	}
	else if (m_pResouceInfo.m_oilAmount < resourceRequestParams.m_oilAmount) {
		return false;
	}
	else if (m_pResouceInfo.m_populationAmount < resourceRequestParams.m_populationAmount) {
		return false;
	}
	else if (m_pResouceInfo.m_wheatAmount < resourceRequestParams.m_wheatAmount) {
		return false;
	}
	else if (m_pResouceInfo.m_flourAmount < resourceRequestParams.m_flourAmount) {
		return false;
	}
	else if (m_pResouceInfo.m_woodAmount < resourceRequestParams.m_woodAmount) {
		return false;
	}
	else if (m_pResouceInfo.m_breadAmount < resourceRequestParams.m_breadAmount) {
		return false;
	}
	return true;
}

void ResourceManagerComponent::RefundResources(SResourceInfo resourceRequestParams)
{
	m_pResouceInfo.m_moneyAmount += resourceRequestParams.m_moneyAmount;
	m_pResouceInfo.m_oilAmount += resourceRequestParams.m_oilAmount;
	m_pResouceInfo.m_populationUsed -= resourceRequestParams.m_populationAmount;
	m_pResouceInfo.m_wheatAmount -= resourceRequestParams.m_wheatAmount;
	m_pResouceInfo.m_flourAmount += resourceRequestParams.m_flourAmount;
	m_pResouceInfo.m_woodAmount += resourceRequestParams.m_woodAmount;
	m_pResouceInfo.m_breadAmount += resourceRequestParams.m_breadAmount;

	m_pResouecesPanelComponent->UpdatePanel();
	CryLog("resource refunded");
}

void ResourceManagerComponent::AddResource(EResourceType type, int32 amount)
{
	switch (type)
	{
	case EResourceType::Money: {
		m_pResouceInfo.m_moneyAmount += amount;
	}break;
	case EResourceType::OIL: {
		m_pResouceInfo.m_oilAmount += amount;
	}break;
	case EResourceType::WHEAT: {
		m_pResouceInfo.m_wheatAmount += amount;
	}break;
	case EResourceType::FLOUR: {
		m_pResouceInfo.m_flourAmount += amount;
	}break;
	case EResourceType::WOOD: {
		m_pResouceInfo.m_woodAmount += amount;
	}break;
	case EResourceType::BREAD: {
		m_pResouceInfo.m_breadAmount += amount;
	}break;
	default:
		break;
	}
	m_pResouecesPanelComponent->UpdatePanel();
}

void ResourceManagerComponent::AddOwnedEntity(IEntity* unit)
{
	this->m_pOwnedEntities.append(unit);
}

void ResourceManagerComponent::RemoveOwnedEntity(IEntity* unit)
{
	DynArray<IEntity*> newArray;
	for (IEntity* entity : m_pOwnedEntities) {
		if (entity != unit) {
			newArray.append(entity);
		}
	}
	m_pOwnedEntities = newArray;
}

void ResourceManagerComponent::SellResource(int32 amount, EResourceType type)
{
	switch (type)
	{
	case EResourceType::OIL: {
		if (m_pResouceInfo.m_oilAmount < amount) {
			CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceManagerComponent : (SellResource) Not Enough Oil to sell");
			return;
		}
		this->m_pResouceInfo.m_oilAmount -= amount * (ResourceManagerComponent::m_oilPrice / 2);
		this->m_pResouceInfo.m_moneyAmount += (amount * ResourceManagerComponent::m_oilPrice / 2);
	}break;
	case EResourceType::WHEAT: {
		if (m_pResouceInfo.m_wheatAmount < amount) {
			CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceManagerComponent : (SellResource) Not Enough Wheat to sell");
			return;
		}
		this->m_pResouceInfo.m_wheatAmount -= amount * (ResourceManagerComponent::m_WheatPrice / 2);
		this->m_pResouceInfo.m_moneyAmount += (amount * ResourceManagerComponent::m_WheatPrice / 2);
	}break;
	case EResourceType::FLOUR: {
		if (m_pResouceInfo.m_flourAmount < amount) {
			CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceManagerComponent : (SellResource) Not Enough Flour to sell");
			return;
		}
		this->m_pResouceInfo.m_flourAmount -= amount * (ResourceManagerComponent::m_FlourPrice / 2);
		this->m_pResouceInfo.m_moneyAmount += (amount * ResourceManagerComponent::m_FlourPrice / 2);
	}break;
	case EResourceType::WOOD: {
		if (m_pResouceInfo.m_woodAmount < amount) {
			CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceManagerComponent : (SellResource) Not Enough Wood to sell");
			return;
		}
		this->m_pResouceInfo.m_woodAmount -= amount * (ResourceManagerComponent::m_WoodPrice / 2);
		this->m_pResouceInfo.m_moneyAmount += (amount * ResourceManagerComponent::m_WoodPrice / 2);
	}break;
	case EResourceType::BREAD: {
		if (m_pResouceInfo.m_breadAmount < amount) {
			CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceManagerComponent : (SellResource) Not Enough Bread to sell");
			return;
		}
		this->m_pResouceInfo.m_breadAmount -= amount * (ResourceManagerComponent::m_BreadPrice / 2);
		this->m_pResouceInfo.m_moneyAmount += (amount * ResourceManagerComponent::m_BreadPrice / 2);
	}break;
	default:
		break;
	}

	m_pResouecesPanelComponent->UpdatePanel();
	m_pAudioComponent->ExecuteTrigger(m_pSellSound);
}

void ResourceManagerComponent::BuyResource(int32 amount, EResourceType type)
{
	int32 buyPrice = 0;
	switch (type)
	{
	case EResourceType::OIL: {
		buyPrice = (amount * ResourceManagerComponent::m_oilPrice);
		if (m_pResouceInfo.m_moneyAmount < buyPrice) {
			CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceManagerComponent : (BuyResource) Not Enough Money to buy");
			return;
		}
		this->m_pResouceInfo.m_oilAmount += amount * ResourceManagerComponent::m_oilPrice;
	}break;
	case EResourceType::WHEAT: {
		buyPrice = (amount * ResourceManagerComponent::m_WheatPrice);
		if (m_pResouceInfo.m_moneyAmount < buyPrice) {
			CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceManagerComponent : (BuyResource) Not Enough Money to buy");
			return;
		}
		this->m_pResouceInfo.m_wheatAmount += amount * ResourceManagerComponent::m_WheatPrice;
	}break;
	case EResourceType::FLOUR: {
		buyPrice = (amount * ResourceManagerComponent::m_FlourPrice);
		if (m_pResouceInfo.m_moneyAmount < buyPrice) {
			CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceManagerComponent : (BuyResource) Not Enough Money to buy");
			return;
		}
		this->m_pResouceInfo.m_flourAmount += amount * ResourceManagerComponent::m_FlourPrice;
	}break;
	case EResourceType::WOOD: {
		buyPrice = (amount * ResourceManagerComponent::m_WoodPrice);
		if (m_pResouceInfo.m_moneyAmount < buyPrice) {
			CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceManagerComponent : (BuyResource) Not Enough Money to buy");
			return;
		}
		this->m_pResouceInfo.m_woodAmount += amount * ResourceManagerComponent::m_WoodPrice;
	}break;
	case EResourceType::BREAD: {
		buyPrice = (amount * ResourceManagerComponent::m_BreadPrice);
		if (m_pResouceInfo.m_moneyAmount < buyPrice) {
			CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceManagerComponent : (BuyResource) Not Enough Money to buy");
			return;
		}
		this->m_pResouceInfo.m_woodAmount += amount * ResourceManagerComponent::m_BreadPrice;
	}break;
	default:
		break;
	}

	this->m_pResouceInfo.m_moneyAmount -= buyPrice;
	m_pResouecesPanelComponent->UpdatePanel();

	m_pAudioComponent->ExecuteTrigger(m_pBuySound);
}

void ResourceManagerComponent::SetIsPlayer(bool isPlayer)
{
	this->bIsPlayer = isPlayer;
}

bool ResourceManagerComponent::IsPlayer()
{
	return bIsPlayer;
}

DynArray<IEntity*> ResourceManagerComponent::GetOwnedEntities()
{
	return m_pOwnedEntities;
}
