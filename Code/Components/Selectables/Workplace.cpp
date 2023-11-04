#include "StdAfx.h"
#include "Workplace.h"
#include "GamePlugin.h"

#include <Components/Info/OwnerInfo.h>
#include <Components/Selectables/Worker.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterWorkplaceComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(WorkplaceComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterWorkplaceComponent);
}

void WorkplaceComponent::Initialize()
{
	for (int32 i = 0; i < m_maxWorkersCount; i++) {
		m_pWorkerEntities[i] = nullptr;
	}
}

Cry::Entity::EventFlags WorkplaceComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void WorkplaceComponent::ProcessEvent(const SEntityEvent& event)
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

void WorkplaceComponent::SetMaxWorkersCount(int32 count)
{
	m_maxWorkersCount = count;
}

int32 WorkplaceComponent::GetCurrentWorkersCount()
{
	int32 count = 0;
	for (int32 i = 0; i < m_maxWorkersCount; i++) {
		if (m_pWorkerEntities[i] == nullptr || m_pWorkerEntities[i]->IsGarbage()) {
			continue;
		}

		count++;
	}
	return count;
}

int32 WorkplaceComponent::AssignWorkerToPlace(IEntity* worker)
{
	//if (GetCurrentWorkersCount() >= m_maxWorkersCount) {
	//	CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "WorkplaceComponent : (AssignWorkerToPlace) Workplace Already full.");
	//	return -1;
	//}

	WorkerComponent* wokerComponent = worker->GetComponent<WorkerComponent>();
	if (!wokerComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "WorkplaceComponent : (AssignWorkerToPlace) Selected Entity is not a worker.");
		return -1;
	}

	for (int32 i = 0; i < m_maxWorkersCount; i++) {
		if (m_pWorkerEntities[i] == nullptr) {
			m_pWorkerEntities[i] = worker;
			return i;
		}
	}

	CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "WorkplaceComponent : (AssignWorkerToPlace) Workplace Already full.");
	return -1;
}

void WorkplaceComponent::RemovedWorkerFromWorkplace(int32 index)
{
	m_pWorkerEntities[index] = nullptr;
}

DynArray<IEntity*> WorkplaceComponent::GetWorkers()
{
	return m_pWorkerEntities;
}

void WorkplaceComponent::SetMoveToAttachments(DynArray<IAttachment*> moveToAttachments)
{
	this->m_moveToAttachments = moveToAttachments;
}

void WorkplaceComponent::SetMoveToAttachment(IAttachment* currentMoveToAttachment)
{
	this->m_currentMoveToAttachment = currentMoveToAttachment;
}

Vec3 WorkplaceComponent::GetCurrentMoveToAttachment()
{
	return ZERO;
}
