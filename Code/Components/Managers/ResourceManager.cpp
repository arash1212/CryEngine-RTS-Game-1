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
	//ResouecesPanelComponent initialization
	m_pResouecesPanelComponent = m_pEntity->GetComponent<UIResourcesPanelComponent>();

	//AudioComponent initialization
	m_pAudioComponent = m_pEntity->GetComponent<IEntityAudioComponent>();

	//Sounds
	m_pBuySound = CryAudio::StringToId("buy_sound_1");
	m_pSellSound = CryAudio::StringToId("sell_sound_1");
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
		m_pResouecesPanelComponent->UpdatePanel();

	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];
		UpdatePopulation();

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
	m_pResouecesPanelComponent->SetPopulationAmount(currentPopulation - m_pResouceInfo.m_populationUsed);
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

	m_pResouceInfo.m_moneyAmount -= resourceRequestParams.m_moneyAmount;
	m_pResouceInfo.m_oilAmount -= resourceRequestParams.m_oilAmount;
	m_pResouceInfo.m_populationUsed += resourceRequestParams.m_populationAmount;
	m_pResouecesPanelComponent->UpdatePanel();
	return true;
}

void ResourceManagerComponent::RefundResources(SResourceInfo resourceRequestParams)
{
	m_pResouceInfo.m_moneyAmount += resourceRequestParams.m_moneyAmount;
	m_pResouceInfo.m_oilAmount += resourceRequestParams.m_oilAmount;
	m_pResouceInfo.m_populationUsed -= resourceRequestParams.m_populationAmount;
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
			CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceManagerComponent : (SellOil) Not Enough Oil to sell");
			return;
		}
		this->m_pResouceInfo.m_oilAmount -= amount * (ResourceManagerComponent::m_oilPrice / 2);
		this->m_pResouceInfo.m_moneyAmount += (amount * ResourceManagerComponent::m_oilPrice / 2);
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
			CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceManagerComponent : (BuyOil) Not Enough Money to buy");
			return;
		}
		this->m_pResouceInfo.m_oilAmount += amount * ResourceManagerComponent::m_oilPrice;
	}break;
	default:
		break;
	}

	this->m_pResouceInfo.m_moneyAmount -= buyPrice;
	m_pResouecesPanelComponent->UpdatePanel();

	m_pAudioComponent->ExecuteTrigger(m_pBuySound);
}
