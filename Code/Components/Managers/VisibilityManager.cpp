#include "StdAfx.h"
#include "VisibilityManager.h"
#include "GamePlugin.h"

#include <Components/Managers/ResourceManager.h>
#include <Utils/EntityUtils.h>

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <Components/Selectables/Visibility.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterVisibilityManagerComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(VisibilityManagerComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterVisibilityManagerComponent);
}


void VisibilityManagerComponent::Initialize()
{

}

Cry::Entity::EventFlags VisibilityManagerComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void VisibilityManagerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {
	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];


		CheckVisibility();
	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

void VisibilityManagerComponent::CheckVisibility()
{
	m_hostilePlayers = EntityUtils::FindHostilePlayers(m_pEntity);
	for (IEntity* player : m_hostilePlayers) {
		ResourceManagerComponent* pOtherResourceManagerComponent = player->GetComponent<ResourceManagerComponent>();
		ResourceManagerComponent* pResourceManagerComponent = m_pEntity->GetComponent<ResourceManagerComponent>();

		for (IEntity* enemyEntity : pOtherResourceManagerComponent->GetOwnedEntities()) {
			for (IEntity* entity : pResourceManagerComponent->GetOwnedEntities()) {
				f32 distanceToEnemyEntity = EntityUtils::GetDistance(entity->GetWorldPos(), enemyEntity->GetWorldPos(), enemyEntity);
				VisibilityComponent* pVisibiliComponent = enemyEntity->GetComponent<VisibilityComponent>();
				if (distanceToEnemyEntity > 30) {
					pVisibiliComponent->SetVisible(false);
				}
				else {
					pVisibiliComponent->SetVisible(true);
				}
			}
		}
	}
}
