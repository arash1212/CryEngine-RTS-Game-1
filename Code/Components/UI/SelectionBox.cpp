#include "StdAfx.h"
#include "SelectionBox.h"
#include "GamePlugin.h"

#include "FlashUI/FlashUI.h"
#include "FlashUI/FlashUIElement.h"

#include <Components/UI/Listener/UIElementEventListener.h>

#include <CryRenderer/IRenderer.h>
#include <CryEntitySystem/IEntitySystem.h>
#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterSelectionBoxComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(SelectionBoxComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterSelectionBoxComponent);
}

void SelectionBoxComponent::Initialize()
{
	//Initialize SelectionBoxUIElement and set it visible
	m_pSelectionBoxUIElement = gEnv->pFlashUI->GetUIElement("selection-box");
	if (m_pSelectionBoxUIElement) {
		m_pSelectionBoxUIElement->SetVisible(true);
	}
	else {
		CryLog("SelectionBoxComponent : (Initialize) m_pSelectionBoxUIElement is null !");
	}
}

Cry::Entity::EventFlags SelectionBoxComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void SelectionBoxComponent::ProcessEvent(const SEntityEvent& event)
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

DynArray<IEntity*> SelectionBoxComponent::GetEntitiesInsideBox(Vec2 lastPoint)
{
	DynArray<IEntity*> resultArray;

	f32 width = crymath::abs(lastPoint.x - m_boxInitPoint.x);
	f32 height = crymath::abs(lastPoint.y - m_boxInitPoint.y);

	f32 minX = lastPoint.x > m_boxInitPoint.x ? m_boxInitPoint.x : lastPoint.x;
	f32 minY = lastPoint.y > m_boxInitPoint.y ? m_boxInitPoint.y : lastPoint.y;
	f32 maxX = minX + width;
	f32 maxY = minY + height;

	int count = 0;
	IEntityItPtr entityPtr = gEnv->pEntitySystem->GetEntityIterator();
	entityPtr->MoveFirst();
	while (!entityPtr->IsEnd())
	{
		IEntity* entity = entityPtr->Next();

		AABB aabb;
		entity->GetWorldBounds(aabb);

		Vec3 center = aabb.GetCenter();
		Vec3 min = aabb.min;
		Vec3 max = aabb.max;

		f32 entityWidth = crymath::abs(max.x - min.x);
		f32 entityHeight = crymath::abs(max.y - min.y);

		Vec3 centerScreenPos = ZERO;
		Vec3 minScreenPos = ZERO;
		Vec3 maxScreenPos = ZERO;

		m_pCameraComponent->GetCamera().Project(center, centerScreenPos);
		m_pCameraComponent->GetCamera().Project(min, minScreenPos);
		m_pCameraComponent->GetCamera().Project(max, maxScreenPos);

		if (
			centerScreenPos.x >= minX && centerScreenPos.x <= maxX && centerScreenPos.y >= minY && centerScreenPos.y <= maxY ||
			minScreenPos.x >= minX && minScreenPos.x <= maxX && minScreenPos.y >= minY && minScreenPos.y <= maxY ||
			maxScreenPos.x >= minX && maxScreenPos.x <= maxX && maxScreenPos.y >= minY && maxScreenPos.y <= maxY ||
			minScreenPos.x + entityWidth >= minX && minScreenPos.x + entityWidth <= maxX && minScreenPos.y + entityHeight >= minY && minScreenPos.y + entityHeight <= maxY) {

			resultArray.append(entity);
			count++;
		}
	}
	return resultArray;
}

void SelectionBoxComponent::SetBoxInitPosition(Vec2 initPoint)
{
	this->m_boxInitPoint = initPoint;
}

void SelectionBoxComponent::SetCameraComponent(Cry::DefaultComponents::CCameraComponent* cameraComponent)
{
	this->m_pCameraComponent = cameraComponent;
}

void SelectionBoxComponent::SetEventListener(IUIElementEventListener* eventListener)
{
	this->m_pSelectionBoxUIElement->AddEventListener(eventListener,"selection-box-event-listener");
}