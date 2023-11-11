#include "StdAfx.h"
#include "UIBuyBulletItem.h"
#include "GamePlugin.h"

#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/ResourceManager.h>
#include <Resources/Resources/BulletResource.h>

UIBuyBulletItem::UIBuyBulletItem(IEntity* entity)
{
	this->m_pEntity = entity;
}

void UIBuyBulletItem::Execute()
{
	if (!m_pEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIBuyBulletItem : (Execute) m_pEntity is null !");
		return;
	}
	m_pOwnerInfoComponent = m_pEntity->GetComponent<OwnerInfoComponent>();
	if (!m_pOwnerInfoComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIBuyBulletItem : (Execute) m_pOwnerInfoComponent is null !");
		return;
	}
	IEntity* m_pOwnerEntity = m_pOwnerInfoComponent->GetOwner();
	if (!m_pOwnerEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIBuyBulletItem : (Execute) m_pOwnerEntity is null !");
		return;
	}
	ResourceManagerComponent* m_pReourceManager = m_pOwnerEntity->GetComponent<ResourceManagerComponent>();
	if (!m_pReourceManager) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UIBuyBulletItem : (Execute) m_pReourceManager is null !");
		return;
	}

	m_pReourceManager->BuyResource(10, EResourceType::BULLET);
}

string UIBuyBulletItem::GetImagePath()
{
	return this->GetDescrption().sBuyIcon;
}

SDescription UIBuyBulletItem::GetDescrption()
{
	BulletResource pBulletResource;
	return pBulletResource.GetDescription();
}