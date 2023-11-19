#include "StdAfx.h"
#include "BaseResourcePoint.h"
#include "GamePlugin.h"

#include <UIItems/IBaseUIItem.h>
#include <UIItems/Items/UICancelItem.h>
#include <UIItems/Items/UIChangeStanceItem.h>
#include <UIItems/Items/Buildings/UIHQ1BuildItem.h>
#include <Components/Selectables/Selectable.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterBaseResourcePointComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(BaseResourcePointComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterBaseResourcePointComponent);
}

void BaseResourcePointComponent::Initialize()
{
}

Cry::Entity::EventFlags BaseResourcePointComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void BaseResourcePointComponent::ProcessEvent(const SEntityEvent& event)
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

Vec3 BaseResourcePointComponent::GetCollectingLocation()
{
	Vec3 pos = m_pCollectingLocationAttachment->GetAttWorldAbsolute().t;
	pos.z = m_pEntity->GetWorldPos().z;
	return pos;
}

void BaseResourcePointComponent::SetType(EResourceType type)
{
	this->m_type = type;
}

EResourceType BaseResourcePointComponent::GetType()
{
	return m_type;
}

void BaseResourcePointComponent::SetIsSingleUse(bool singleUse)
{
	this->bIsSingleUse = singleUse;
}

bool BaseResourcePointComponent::IsSingleUse()
{
	return bIsSingleUse;
}

bool BaseResourcePointComponent::IsInUse()
{
	return bIsInUse;
}

void BaseResourcePointComponent::SetIsInUse(bool inUse)
{
	this->bIsInUse = inUse;
}

void BaseResourcePointComponent::SetCurrentCollector(IEntity* currentCollector)
{
	this->m_pCurrentCollector = currentCollector;
}

IEntity* BaseResourcePointComponent::GetCurrentCollector()
{
	return m_pCurrentCollector;
}

void BaseResourcePointComponent::SetHasCollectingLocation(bool hasCollectingLocation)
{
	this->bHasCollectingLocation = hasCollectingLocation;
}

bool BaseResourcePointComponent::HasCollectingLocation()
{
	return this->bHasCollectingLocation;
}

void BaseResourcePointComponent::AddCollector(IEntity* collectorEntity)
{
	this->m_collectors.append(collectorEntity);
}

void BaseResourcePointComponent::RemoveCollector(IEntity* collectorEntity)
{
	DynArray<IEntity*> result;
	for (IEntity* entity : m_collectors) {
		if (entity != collectorEntity) {
			result.append(entity);
		}
	}

	m_collectors.clear();
	m_collectors = result;
}

DynArray<IEntity*> BaseResourcePointComponent::GetCollectors()
{
	return m_collectors;
}

