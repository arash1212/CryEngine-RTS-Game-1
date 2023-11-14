#include "StdAfx.h"
#include "UIBuyOilItem.h"
#include "GamePlugin.h"

#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/ResourceManager.h>
#include <Resources/Resources/OilResource.h>

UIBuyOilItem::UIBuyOilItem(IEntity* entity)
{
	this->m_pEntity = entity;
}

void UIBuyOilItem::Execute()
{
	if (!m_pEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UISellOilItem : (Execute) m_pEntity is null !");
		return;
	}
	m_pOwnerInfoComponent = m_pEntity->GetComponent<OwnerInfoComponent>();
	if (!m_pOwnerInfoComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UISellOilItem : (Execute) m_pOwnerInfoComponent is null !");
		return;
	}
	IEntity* m_pOwnerEntity = m_pOwnerInfoComponent->GetOwner();
	if (!m_pOwnerEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UISellOilItem : (Execute) m_pOwnerEntity is null !");
		return;
	}
	ResourceManagerComponent* m_pReourceManager = m_pOwnerEntity->GetComponent<ResourceManagerComponent>();
	if (!m_pReourceManager) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UISellOilItem : (Execute) m_pReourceManager is null !");
		return;
	}

	m_pReourceManager->BuyResource(10, EResourceType::OIL);
}

string UIBuyOilItem::GetImagePath()
{
	return  this->GetDescrption().sBuyIcon;
}

SDescription UIBuyOilItem::GetDescrption()
{
	return RESOURCE_OIL->GetDescription();
}