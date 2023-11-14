#include "StdAfx.h"
#include "UIBuyIronItem.h"
#include "GamePlugin.h"

#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/ResourceManager.h>
#include <Resources/Resources/IronResource.h>

UIBuyIronItem::UIBuyIronItem(IEntity* entity)
{
	this->m_pEntity = entity;
}

void UIBuyIronItem::Execute()
{
	if (!m_pEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIBuyIronItem : (Execute) m_pEntity is null !");
		return;
	}
	m_pOwnerInfoComponent = m_pEntity->GetComponent<OwnerInfoComponent>();
	if (!m_pOwnerInfoComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIBuyIronItem : (Execute) m_pOwnerInfoComponent is null !");
		return;
	}
	IEntity* m_pOwnerEntity = m_pOwnerInfoComponent->GetOwner();
	if (!m_pOwnerEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIBuyIronItem : (Execute) m_pOwnerEntity is null !");
		return;
	}
	ResourceManagerComponent* m_pReourceManager = m_pOwnerEntity->GetComponent<ResourceManagerComponent>();
	if (!m_pReourceManager) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIBuyIronItem : (Execute) m_pReourceManager is null !");
		return;
	}

	m_pReourceManager->BuyResource(10, EResourceType::IRON);
}

string UIBuyIronItem::GetImagePath()
{
	return this->GetDescrption().sBuyIcon;
}

SDescription UIBuyIronItem::GetDescrption()
{
	return RESOURCE_IRON->GetDescription();
}
