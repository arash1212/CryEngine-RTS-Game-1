#include "StdAfx.h"
#include "UISellBreadItem.h"
#include "GamePlugin.h"

#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/ResourceManager.h>
#include <Resources/Resources/BreadResource.h>

UISellBreadItem::UISellBreadItem(IEntity* entity)
{
	this->m_pEntity = entity;
}

void UISellBreadItem::Execute()
{
	if (!m_pEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UISellBreadItem : (Execute) m_pEntity is null !");
		return;
	}
	m_pOwnerInfoComponent = m_pEntity->GetComponent<OwnerInfoComponent>();
	if (!m_pOwnerInfoComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UISellBreadItem : (Execute) m_pOwnerInfoComponent is null !");
		return;
	}
	IEntity* m_pOwnerEntity = m_pOwnerInfoComponent->GetOwner();
	if (!m_pOwnerEntity) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UISellBreadItem : (Execute) m_pOwnerEntity is null !");
		return;
	}
	ResourceManagerComponent* m_pReourceManager = m_pOwnerEntity->GetComponent<ResourceManagerComponent>();
	if (!m_pReourceManager) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "UISellBreadItem : (Execute) m_pReourceManager is null !");
		return;
	}

	m_pReourceManager->SellResource(10, EResourceType::BREAD);
}

string UISellBreadItem::GetImagePath()
{
	return  this->GetDescrption().sSellIcon;
}

SDescription UISellBreadItem::GetDescrption()
{
	BreadResource pBreadResource;
	return pBreadResource.GetDescription();
}