#include "StdAfx.h"
#include "ResourceCollector.h"
#include "GamePlugin.h"

#include <UIItems/IBaseUIItem.h>
#include <UIItems/Items/UICancelItem.h>
#include <UIItems/Items/UIChangeStanceItem.h>
#include <UIItems/Items/Buildings/UIHQ1BuildItem.h>
#include <Components/Info/OwnerInfo.h>
#include <Components/Managers/ResourceManager.h>

#include <Components/Info/OwnerInfo.h>
#include <Resources/IResource.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterResourceCollectorComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(ResourceCollectorComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterResourceCollectorComponent);
}

void ResourceCollectorComponent::Initialize()
{
	//AnimationComponent Initializations
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();

	//OwnerComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetOrCreateComponent<OwnerInfoComponent>();

	//**************************************Resource Attchments
	//OilBarrelAttachment Initialization
	m_pOilBarrelAttachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("oilBarrel");
	m_pWheatAttachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("wheat");
	m_pFlourAttachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("flour");
	m_pWoodAttachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("wood");
	m_pBreadAttachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("bread");
	m_pSulfurAttachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("sulfur");
	m_pGunPowderAttachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("gunPowder");
	m_pIronAttachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("iron");
	m_pBulletAttachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("bullet");
	m_pAK47Attachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("ak47");
}

Cry::Entity::EventFlags ResourceCollectorComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void ResourceCollectorComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];

		UpdateResourceAttachment();

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

void ResourceCollectorComponent::AddResource(int32 amount)
{
	m_amountResourceCollected += amount;
}

int32 ResourceCollectorComponent::GetAmountOfResourceCollected()
{
	return m_amountResourceCollected;
}

void ResourceCollectorComponent::SendResourceToWareHouse()
{
	if (!m_pOwnerInfoComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceCollectorComponent: (SendResourceToWareHouse) OwnerInfoComponent cannot be null.");
		return;
	}
	IEntity* owner = m_pOwnerInfoComponent->GetOwner();
	if (!owner) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceCollectorComponent: (SendResourceToWareHouse) owner cannot be null.");
		return;
	}
	ResourceManagerComponent* resourceManager = owner->GetComponent<ResourceManagerComponent>();
	if (!resourceManager) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "ResourceCollectorComponent: (SendResourceToWareHouse) resourceManager cannot be null.");
		return;
	}

	switch (m_pCurrentResourceType)
	{
	case EResourceType::Money: {
		resourceManager->AddResource(EResourceType::Money, m_amountResourceCollected);
	}break;
	case EResourceType::OIL: {
		resourceManager->AddResource(EResourceType::OIL, m_amountResourceCollected);
	}break;
	case EResourceType::WHEAT: {
		resourceManager->AddResource(EResourceType::WHEAT, m_amountResourceCollected);
	}break;
	case EResourceType::WOOD: {
		resourceManager->AddResource(EResourceType::WOOD, m_amountResourceCollected);
	}break;
	case EResourceType::SULFUR: {
		resourceManager->AddResource(EResourceType::SULFUR, m_amountResourceCollected);
	}break;
	case EResourceType::GUN_POWDER: {
		resourceManager->AddResource(EResourceType::GUN_POWDER, m_amountResourceCollected);
	}break;
	case EResourceType::IRON: {
		resourceManager->AddResource(EResourceType::IRON, m_amountResourceCollected);
	}break;
	case EResourceType::BULLET: {
		resourceManager->AddResource(EResourceType::BULLET, m_amountResourceCollected);
	}break;
	case EResourceType::AK47: {
		resourceManager->AddResource(EResourceType::BULLET, m_amountResourceCollected);
	}break;
	default:
		break;
	}

	this->m_amountResourceCollected = 0;
}

void ResourceCollectorComponent::UpdateResourceAttachment()
{
	EResourceType pResourceType = GetCurrentResourceType();
	if (GetAmountOfResourceCollected() <= 0) {
		m_pOilBarrelAttachment->HideAttachment(true);
		m_pWheatAttachment->HideAttachment(true);
		m_pFlourAttachment->HideAttachment(true);
		m_pWoodAttachment->HideAttachment(true);
		m_pBreadAttachment->HideAttachment(true);
		m_pSulfurAttachment->HideAttachment(true);
		m_pGunPowderAttachment->HideAttachment(true);
		m_pIronAttachment->HideAttachment(true);
		m_pBulletAttachment->HideAttachment(true);
		m_pAK47Attachment->HideAttachment(true);
		return;
	}

	switch (pResourceType)
	{
	case EResourceType::Money: {
		m_pOilBarrelAttachment->HideAttachment(true);
		m_pWheatAttachment->HideAttachment(true);
		m_pFlourAttachment->HideAttachment(true);
		m_pWoodAttachment->HideAttachment(true);
		m_pBreadAttachment->HideAttachment(true);
		m_pSulfurAttachment->HideAttachment(true);
		m_pGunPowderAttachment->HideAttachment(true);
		m_pIronAttachment->HideAttachment(true);
		m_pBulletAttachment->HideAttachment(true);
		m_pAK47Attachment->HideAttachment(true);
	}break;
	case EResourceType::OIL: {
		m_pOilBarrelAttachment->HideAttachment(false);
		m_pWheatAttachment->HideAttachment(true);
		m_pFlourAttachment->HideAttachment(true);
		m_pWoodAttachment->HideAttachment(true);
		m_pBreadAttachment->HideAttachment(true);
		m_pSulfurAttachment->HideAttachment(true);
		m_pGunPowderAttachment->HideAttachment(true);
		m_pIronAttachment->HideAttachment(true);
		m_pBulletAttachment->HideAttachment(true);
		m_pAK47Attachment->HideAttachment(true);
	}break;
	case EResourceType::WHEAT: {
		m_pOilBarrelAttachment->HideAttachment(true);
		m_pWheatAttachment->HideAttachment(false);
		m_pFlourAttachment->HideAttachment(true);
		m_pWoodAttachment->HideAttachment(true);
		m_pBreadAttachment->HideAttachment(true);
		m_pSulfurAttachment->HideAttachment(true);
		m_pGunPowderAttachment->HideAttachment(true);
		m_pIronAttachment->HideAttachment(true);
		m_pBulletAttachment->HideAttachment(true);
		m_pAK47Attachment->HideAttachment(true);
	}break;
	case EResourceType::FLOUR: {
		m_pOilBarrelAttachment->HideAttachment(true);
		m_pWheatAttachment->HideAttachment(true);
		m_pFlourAttachment->HideAttachment(false);
		m_pWoodAttachment->HideAttachment(true);
		m_pBreadAttachment->HideAttachment(true);
		m_pSulfurAttachment->HideAttachment(true);
		m_pGunPowderAttachment->HideAttachment(true);
		m_pIronAttachment->HideAttachment(true);
		m_pBulletAttachment->HideAttachment(true);
		m_pAK47Attachment->HideAttachment(true);
	}break;
	case EResourceType::WOOD: {
		m_pOilBarrelAttachment->HideAttachment(true);
		m_pWheatAttachment->HideAttachment(true);
		m_pFlourAttachment->HideAttachment(true);
		m_pWoodAttachment->HideAttachment(false);
		m_pBreadAttachment->HideAttachment(true);
		m_pSulfurAttachment->HideAttachment(true);
		m_pGunPowderAttachment->HideAttachment(true);
		m_pIronAttachment->HideAttachment(true);
		m_pBulletAttachment->HideAttachment(true);
		m_pAK47Attachment->HideAttachment(true);
	}break;
	case EResourceType::BREAD: {
		m_pOilBarrelAttachment->HideAttachment(true);
		m_pWheatAttachment->HideAttachment(true);
		m_pFlourAttachment->HideAttachment(true);
		m_pWoodAttachment->HideAttachment(true);
		m_pBreadAttachment->HideAttachment(false);
		m_pSulfurAttachment->HideAttachment(true);
		m_pGunPowderAttachment->HideAttachment(true);
		m_pIronAttachment->HideAttachment(true);
		m_pBulletAttachment->HideAttachment(true);
		m_pAK47Attachment->HideAttachment(true);
	}break;
	case EResourceType::SULFUR: {
		m_pOilBarrelAttachment->HideAttachment(true);
		m_pWheatAttachment->HideAttachment(true);
		m_pFlourAttachment->HideAttachment(true);
		m_pWoodAttachment->HideAttachment(true);
		m_pBreadAttachment->HideAttachment(true);
		m_pSulfurAttachment->HideAttachment(false);
		m_pGunPowderAttachment->HideAttachment(true);
		m_pIronAttachment->HideAttachment(true);
		m_pBulletAttachment->HideAttachment(true);
		m_pAK47Attachment->HideAttachment(true);
	}break;
	case EResourceType::GUN_POWDER: {
		m_pOilBarrelAttachment->HideAttachment(true);
		m_pWheatAttachment->HideAttachment(true);
		m_pFlourAttachment->HideAttachment(true);
		m_pWoodAttachment->HideAttachment(true);
		m_pBreadAttachment->HideAttachment(true);
		m_pSulfurAttachment->HideAttachment(true);
		m_pGunPowderAttachment->HideAttachment(false);
		m_pIronAttachment->HideAttachment(true);
		m_pBulletAttachment->HideAttachment(true);
		m_pAK47Attachment->HideAttachment(true);
	}break;
	case EResourceType::IRON: {
		m_pOilBarrelAttachment->HideAttachment(true);
		m_pWheatAttachment->HideAttachment(true);
		m_pFlourAttachment->HideAttachment(true);
		m_pWoodAttachment->HideAttachment(true);
		m_pBreadAttachment->HideAttachment(true);
		m_pSulfurAttachment->HideAttachment(true);
		m_pGunPowderAttachment->HideAttachment(true);
		m_pIronAttachment->HideAttachment(false);
		m_pBulletAttachment->HideAttachment(true);
		m_pAK47Attachment->HideAttachment(true);
	}break;
	case EResourceType::BULLET: {
		m_pOilBarrelAttachment->HideAttachment(true);
		m_pWheatAttachment->HideAttachment(true);
		m_pFlourAttachment->HideAttachment(true);
		m_pWoodAttachment->HideAttachment(true);
		m_pBreadAttachment->HideAttachment(true);
		m_pSulfurAttachment->HideAttachment(true);
		m_pGunPowderAttachment->HideAttachment(true);
		m_pIronAttachment->HideAttachment(true);
		m_pBulletAttachment->HideAttachment(false);
		m_pAK47Attachment->HideAttachment(true);
	}break;
	case EResourceType::AK47: {
		m_pOilBarrelAttachment->HideAttachment(true);
		m_pWheatAttachment->HideAttachment(true);
		m_pFlourAttachment->HideAttachment(true);
		m_pWoodAttachment->HideAttachment(true);
		m_pBreadAttachment->HideAttachment(true);
		m_pSulfurAttachment->HideAttachment(true);
		m_pGunPowderAttachment->HideAttachment(true);
		m_pIronAttachment->HideAttachment(true);
		m_pBulletAttachment->HideAttachment(true);
		m_pAK47Attachment->HideAttachment(false);
	}break;
	default:
		break;
	}
}

int32 ResourceCollectorComponent::GetMaxResourceCanBeCollected()
{
	return m_maxResouceCanBeCollected;
}

void ResourceCollectorComponent::SetCurrentResourceType(EResourceType resourceType)
{
	m_pCurrentResourceType = resourceType;
}

EResourceType ResourceCollectorComponent::GetCurrentResourceType()
{
	return m_pCurrentResourceType;
}

bool ResourceCollectorComponent::CanCollectResource()
{
	return m_amountResourceCollected < m_maxResouceCanBeCollected;
}

void ResourceCollectorComponent::EmptyResources()
{
	m_amountResourceCollected = 0;
}
