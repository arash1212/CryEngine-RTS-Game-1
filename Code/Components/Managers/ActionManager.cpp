#include "StdAfx.h"
#include "ActionManager.h"
#include "GamePlugin.h"

#include <Actions/IBaseAction.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterActionManagerComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(ActionManagerComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterActionManagerComponent);
}

void ActionManagerComponent::Initialize()
{

}

Cry::Entity::EventFlags ActionManagerComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void ActionManagerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];

		this->ProcessActions();

	}break;
	case Cry::Entity::EEvent::Reset: {
		m_actionsQueue.clear();

	}break;
	default:
		break;
	}
}

void ActionManagerComponent::ProcessActions()
{
	if (!m_actionsQueue.empty()) {
		if (!m_pCurrentAction) {
			m_pCurrentAction = m_actionsQueue.front();
			return;
		}
		else if (m_pCurrentAction && m_pCurrentAction->IsDone()) {
			m_actionsQueue.pop_front();
			m_pCurrentAction = nullptr;
			return;
		}
		else if (m_pCurrentAction && !m_pCurrentAction->IsDone()) {
			m_pCurrentAction->Execute();
		}
	}
	else {
		m_pCurrentAction = nullptr;
	}
}

void ActionManagerComponent::AddAction(IBaseAction* action)
{
	//TODO
	if (m_pCurrentAction) {
		m_pCurrentAction->Cancel();
		m_pCurrentAction = nullptr;
	}

	m_actionsQueue.clear();
	m_actionsQueue.push_back(action);
}

void ActionManagerComponent::CancelCurrentAction()
{
	if (!m_pCurrentAction) {
		return;
	}
	this->m_pCurrentAction->Cancel();
	//this->m_actionsQueue.clear();
}
