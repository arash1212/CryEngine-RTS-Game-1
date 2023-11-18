#include "StdAfx.h"
#include "GuardPost.h"
#include "GamePlugin.h"

#include <Components/Info/OwnerInfo.h>
#include <Components/BaseBuilding/Building.h>
#include <Components/Managers/ResourceManager.h>
#include <Components/Selectables/Health.h>
#include <Components/Selectables/Guard.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterGuardPostComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(GuardPostComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterGuardPostComponent);
}

void GuardPostComponent::Initialize()
{
	//BuildingComponent Initialization
	m_pBuildingComponent = m_pEntity->GetComponent<BuildingComponent>();

	for (int32 i = 0; i < m_maxGuardsCount; i++) {
		m_pGuardEntities.append(nullptr);
	}
}

Cry::Entity::EventFlags GuardPostComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void GuardPostComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];

		UpdateGuardPositions();

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

void GuardPostComponent::SetMaxGuardsCount(int32 count)
{
	m_maxGuardsCount = count;
}

int32 GuardPostComponent::GetMaxGuardsCount()
{
	return m_maxGuardsCount;
}

int32 GuardPostComponent::GetCurrentGuardsCount()
{
	int32 count = 0;
	for (int32 i = 0; i < m_maxGuardsCount; i++) {
		if (m_pGuardEntities[i] == nullptr) {
			continue;
		}

		count++;
	}

	return count;
}

int32 GuardPostComponent::AssignGuardToPlace(IEntity* guard)
{
	if (!m_pBuildingComponent->IsBuilt()) {
		return -1;
	}
	if (GetCurrentGuardsCount() >= m_maxGuardsCount) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "GuardPostComponent : (AssignGuardToPlace) GuardPost Already full.");
		//return -1;
	}

	GuardComponent* pGuardComponent = guard->GetComponent<GuardComponent>();
	if (!pGuardComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "GuardPostComponent : (AssignGuardToPlace) Selected Entity is not a worker.");
		return -1;
	}

	//TODO : khata dasht ye dafe worker be tamam workplace ha assign mishod (az inja) felan okeye (?)
	for (int32 i = 0; i < m_maxGuardsCount; i++) {
		if (m_pGuardEntities[i] == nullptr || m_pGuardEntities[i]->IsGarbage()) {
			m_pGuardEntities[i] = guard;
			UpdateGuardaSlotsUI();
			return i;
		}
	}

	CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "GuardPostComponent : (AssignGuardToPlace) GuardPost Already full.");
	return -1;
}

void GuardPostComponent::RemovedGuardFromWorkplace(int32 index)
{
	m_pGuardEntities[index] = nullptr;
	UpdateGuardaSlotsUI();
}

DynArray<IEntity*> GuardPostComponent::GetGuards()
{
	return m_pGuardEntities;
}

DynArray<IAttachment*> GuardPostComponent::GetGuardPointAttachments()
{
	return m_guardPointAttachments;
}

void GuardPostComponent::SetGuardPointAttachments(DynArray<IAttachment*> guardPointAttachments)
{
	this->m_guardPointAttachments = guardPointAttachments;
}

void GuardPostComponent::UpdateGuardPositions()
{
	for (int32 i = 0; i < m_pGuardEntities.size(); i++) {
		IEntity* pGuardEntity = m_pGuardEntities[i];
		if (!pGuardEntity) {
			return;
		}
		GuardComponent* pGuardComponent = pGuardEntity->GetComponent<GuardComponent>();
		if (!pGuardComponent || !pGuardComponent->HasEnteredGuardPost()) {
			continue;
		}
		pGuardEntity->SetPos(m_guardPointAttachments[i]->GetAttWorldAbsolute().t);
	}
}

void GuardPostComponent::UpdateGuardaSlotsUI()
{
	HealthComponent* pHealthComponent = m_pEntity->GetComponent<HealthComponent>();
	if (!pHealthComponent) {
		return;
	}

	pHealthComponent->ClearWorkersSlots();
	for (int32 i = 0; i < m_maxGuardsCount; i++) {
		pHealthComponent->AddWorkerSlot(i, m_pGuardEntities[i] != nullptr);
	}
}