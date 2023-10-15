#include "StdAfx.h"
#include "Player.h"
#include "GamePlugin.h"

#include <UIItems/IBaseUIItem.h>
#include <Components/UI/SelectionBox.h>
#include <Components/UI/Actionbar.h>
#include <Components/UI/Listener/UIElementEventListener.h>

#include <Utils\MouseUtils.h>
#include <Components/Selectables/Selectable.h>
#include <Components/Selectables/Engineer.h>

#include <Components/Managers/ActionManager.h>
#include <Actions/Units/MoveAction.h>
#include <Actions/Units/AttackAction.h>
#include <Actions/Units/UnitBuildAction.h>
#include <Components/Info/OwnerInfo.h>
#include <Components/BaseBuilding/Building.h>

#include <Components/BaseBuilding/BaseBuilding.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterPlayerComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(PlayerComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPlayerComponent);
}

void PlayerComponent::Initialize()
{
	//Camera initialization
	m_pCameraComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCameraComponent>();
	m_pCameraComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), Quat::CreateRotationX(DEG2RAD(-70)), Vec3(0, 0, m_cameraDefaultHeight)));

	//Inputs initialization
	m_pInputComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();
	InitInputs();

	m_defaultPosZ = m_pEntity->GetPos().z;

	//UIElementEventListener initialization
	m_pUIElementEventListener = new UIElementEventListener(this);

	//SelectionBox component initialization
	m_pSelectionBoxComponent = m_pEntity->GetOrCreateComponent<SelectionBoxComponent>();
	m_pSelectionBoxComponent->SetCameraComponent(m_pCameraComponent);
	m_pSelectionBoxComponent->SetEventListener(m_pUIElementEventListener);

	//Actionbar component initialization
	m_pActionbarComponent = m_pEntity->GetOrCreateComponent<ActionbarComponent>();
	m_pActionbarComponent->SetEventListener(m_pUIElementEventListener);

	//BaseBuildingComponent initialization
	m_pBaseBuildingComponent = m_pEntity->GetOrCreateComponent<BaseBuildingComponent>();

	//ListenerComponent initialization
	m_pListenerComp = m_pEntity->GetOrCreateComponent<Cry::Audio::DefaultComponents::CListenerComponent>();

	//OwnerInfoComponent initialization
	m_pOwnerInfoComponent = m_pEntity->GetOrCreateComponent<OwnerInfoComponent>();
	m_pOwnerInfoComponent->SetIsPlayer(true);

	//Set player entity name
	m_pEntity->SetName(PLAYER_ENTITY_NAME);
}

Cry::Entity::EventFlags PlayerComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void PlayerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		f32 DeltaTime = event.fParam[0];

		Move(DeltaTime);

		CheckSelectablesMouseOver();

		if (m_rightClickCountRestartTimePassed < m_timeBetweenRightClickCountRestart) {
			m_rightClickCountRestartTimePassed += 0.5f * DeltaTime;
		}
		else {
			m_rightClickCount = 0;
		}

	}break;
	case Cry::Entity::EEvent::Reset: {
		DeselectSelectables();

	}break;
	default:
		break;
	}
}

void PlayerComponent::Move(f32 DeltaTime)
{
	Vec3 currentPos = m_pEntity->GetWorldPos();

	//Apply movement
	Vec3 pos = currentPos + (m_movementOffset * m_cameraMoveSpeed * DeltaTime);

	//Apply zoomAmount
	pos.z = m_defaultPosZ - m_currentZoomAmount;

	m_pEntity->SetPos(pos);
}

/*=============================================================================================================================================
																	INPUTS													
==============================================================================================================================================*/

void PlayerComponent::InitInputs()
{
	//MoveForward
	m_pInputComponent->RegisterAction("player", "forward", [this](int activationMode, float value) {this->MoveForward(activationMode, value); });
	m_pInputComponent->BindAction("player", "forward", eAID_KeyboardMouse, eKI_W);

	//MoveBackward
	m_pInputComponent->RegisterAction("player", "backward", [this](int activationMode, float value) {this->MoveBackward(activationMode, value); });
	m_pInputComponent->BindAction("player", "backward", eAID_KeyboardMouse, eKI_S);

	//MoveRight
	m_pInputComponent->RegisterAction("player", "right", [this](int activationMode, float value) {this->MoveRight(activationMode, value); });
	m_pInputComponent->BindAction("player", "right", eAID_KeyboardMouse, eKI_D);

	//MoveLeft
	m_pInputComponent->RegisterAction("player", "left", [this](int activationMode, float value) {this->MoveLeft(activationMode, value); });
	m_pInputComponent->BindAction("player", "left", eAID_KeyboardMouse, eKI_A);

	//Camera ZoomIn
	m_pInputComponent->RegisterAction("player", "zoomIn", [this](int activationMode, float value) {this->MouseWheelUp(activationMode, value); });
	m_pInputComponent->BindAction("player", "zoomIn", eAID_KeyboardMouse, eKI_MouseWheelUp);

	//Camera ZoomOut
	m_pInputComponent->RegisterAction("player", "zoomOut", [this](int activationMode, float value) {this->MouseWheelDown(activationMode, value); });
	m_pInputComponent->BindAction("player", "zoomOut", eAID_KeyboardMouse, eKI_MouseWheelDown);

	//Selectiom
	m_pInputComponent->RegisterAction("player", "select", [this](int activationMode, float value) {this->LeftMouseDown(activationMode, value); });
	m_pInputComponent->BindAction("player", "select", eAID_KeyboardMouse, eKI_Mouse1);

	//Command
	m_pInputComponent->RegisterAction("player", "command", [this](int activationMode, float value) {this->RightMouseDown(activationMode, value); });
	m_pInputComponent->BindAction("player", "command", eAID_KeyboardMouse, eKI_Mouse2);

}

void PlayerComponent::MoveForward(int activationMode, float value)
{
	m_movementOffset.y = value * m_movementSpeed * gEnv->pTimer->GetFrameTime();
}

void PlayerComponent::MoveBackward(int activationMode, float value)
{
	m_movementOffset.y = -value * m_movementSpeed * gEnv->pTimer->GetFrameTime();
}

void PlayerComponent::MoveRight(int activationMode, float value)
{
	m_movementOffset.x = value * m_movementSpeed * gEnv->pTimer->GetFrameTime();
}

void PlayerComponent::MoveLeft(int activationMode, float value)
{
	m_movementOffset.x = -value * m_movementSpeed * gEnv->pTimer->GetFrameTime();
}

void PlayerComponent::MouseWheelUp(int activationMode, float value)
{
	m_currentZoomAmount = CLAMP(m_currentZoomAmount + (value * gEnv->pTimer->GetFrameTime()), 0, m_cameraMaxZoomAmount);
}

void PlayerComponent::MouseWheelDown(int activationMode, float value)
{
	m_currentZoomAmount = CLAMP(m_currentZoomAmount + (value * gEnv->pTimer->GetFrameTime()), 0, m_cameraMaxZoomAmount);
}

void PlayerComponent::LeftMouseDown(int activationMode, float value)
{
	if (!MouseUtils::IsMouseInsideViewPort()) {
		return;
	}

	Vec2 mousePos = MouseUtils::GetCursorPosition();

	if (activationMode == eAAM_OnPress) {
		m_pSelectionBoxComponent->SetBoxInitPosition(mousePos);
	}

	if (activationMode == eAAM_OnRelease) {
		if (m_pUIElementEventListener->IsMouseOverUI()) {
			return;
		}

		///////////////////////////Building
		if (m_pBaseBuildingComponent && m_pBaseBuildingComponent->HasBuildingAssigned()) {
			IEntity* buildingEntity = m_pBaseBuildingComponent->PlaceBuilding(MouseUtils::GetPositionUnderCursor());
			if (buildingEntity) {
				AssignBuildingToEngineers(buildingEntity);
			}
			return;
		}
		///////////////////////////


		//////////////////////////Selection
		DeselectSelectables();

		//Single selection
		IEntity* entity = MouseUtils::GetActorUnderCursor();
		if (entity && !m_pSelectionBoxComponent->IsBoxSelectionTriggered(mousePos)) {
			m_selectedUnits.push_back(entity);
			SelectSelectables();
		}

		//Box/Multiple Selection
		else if (m_pSelectionBoxComponent->IsBoxSelectionTriggered(mousePos)) {
			m_selectedUnits = m_pSelectionBoxComponent->GetEntitiesInsideBox(mousePos);
			SelectSelectables();
		}
		//////////////////////////

		//Actionbar
		AddUIItemsToActionbar();
	}
}

void PlayerComponent::RightMouseDown(int activationMode, float value)
{
	if (!MouseUtils::IsMouseInsideViewPort()) {
		return;
	}

	Vec3 mousePos = MouseUtils::GetPositionUnderCursor();

	if (activationMode == eAAM_OnRelease) {
		///////////////////////////Building
		if (m_pBaseBuildingComponent->HasBuildingAssigned()) {
			m_pBaseBuildingComponent->CancelAssignedBuilding();
			return;
		}
		///////////////////////////

		///////////////////////////Commands
		m_rightClickCount++;
		m_rightClickCountRestartTimePassed = 0;
		//TODO : update beshe
		IEntity* entity = MouseUtils::GetActorUnderCursor();
		if (entity) {
			if (m_pOwnerInfoComponent->IsEntityHostile(entity)) {
				SetUnitsAttackTarget(entity);
				return;
			}

			//
			BuildingComponent* building = entity->GetComponent<BuildingComponent>();
			if (building && !building->IsBuilt()) {
				AssignBuildingToEngineers(entity);
				return;
			}
		}
		else {
			CommandUnitsToMove(mousePos);
		}
		///////////////////////////
	}
}

/*=============================================================================================================================================
																	ACTIONS
==============================================================================================================================================*/

void PlayerComponent::DeselectSelectables()
{
	for (IEntity* entity : m_selectedUnits) {
		SelectableComponent* selectable = entity->GetComponent<SelectableComponent>();
		if (selectable) {
			selectable->DeSelect();
		}
		else {
			continue;
		}
	}

	m_selectedUnits.clear();
}

void PlayerComponent::SelectSelectables()
{
	for (IEntity* entity : m_selectedUnits) {
		SelectableComponent* selectable = entity->GetComponent<SelectableComponent>();
		if (selectable) {
			selectable->Select();
		}
		else {
			continue;
		}
	}
}

void PlayerComponent::CommandUnitsToMove(Vec3 position)
{
	int32 row = 0, column = 0;
	for (int32 i = 0; i < m_selectedUnits.size(); i++) {
		ActionManagerComponent* actionManager = m_selectedUnits[i]->GetComponent<ActionManagerComponent>();
		if (actionManager) {
			if (i != 0) {
				column++;
			}
			if (i != 0 && i % 5 == 0) {
				row++;
				column = 0;
			}

			AABB aabb;
			m_selectedUnits[i]->GetWorldBounds(aabb);
			f32 width = aabb.max.x - aabb.min.x;
			f32 height = aabb.max.y - aabb.min.y;
			Vec3 pos = Vec3(position.x + ((column * width) + 0.8f), position.y - ((row * height) + 0.8f), position.z);
			actionManager->AddAction(new MoveAction(m_selectedUnits[i], pos, m_rightClickCount >= 2));
		}
		else {
			continue;
		}
	}
}

void PlayerComponent::SetUnitsAttackTarget(IEntity* target)
{
	for (IEntity* entity : m_selectedUnits) {
		ActionManagerComponent* actionManager = entity->GetComponent<ActionManagerComponent>();
		if (actionManager) {
			if (target) {
				actionManager->AddAction(new AttackAction(entity, target));
			}
		}
		else {
			continue;
		}
	}
}

void PlayerComponent::AssignBuildingToEngineers(IEntity* buildingEntity)
{
	for (IEntity* entity : m_selectedUnits) {
		if (!entity->GetComponent<EngineerComponent>()) {
			continue;
		}
		ActionManagerComponent* actionManager = entity->GetComponent<ActionManagerComponent>();
		if (actionManager) {
			if (buildingEntity) {
				actionManager->AddAction(new UnitBuildAction(entity, buildingEntity));
			}
		}
		else {
			continue;
		}
	}
}

void PlayerComponent::AddUIItemsToActionbar()
{
	m_pActionbarComponent->Clear();
	if (m_selectedUnits.size() <= 0) {
		return;
	}

	//TODO : UPDATE BESHE
	if (m_selectedUnits.size() > 1) {
		SelectableComponent* selectable = m_selectedUnits[0]->GetComponent<SelectableComponent>();
		if (selectable) {
			for (IBaseUIItem* uiItem : selectable->GetGeneralUIItems()) {
				m_pActionbarComponent->AddButton(uiItem->GetImagePath());
			}
			return;
		}
	}

	if (m_selectedUnits.size() == 1) {
		SelectableComponent* selectable = m_selectedUnits[0]->GetComponent<SelectableComponent>();
		if (selectable) {
			for (IBaseUIItem* uiItem : selectable->GetAllUIItems()) {
				m_pActionbarComponent->AddButton(uiItem->GetImagePath());
			}
		}
	}
}

void PlayerComponent::CheckSelectablesMouseOver()
{
	//Turn highligh color to green on entity if it's selectable
	IEntity* entity = MouseUtils::GetActorUnderCursor();
	if (entity) {
		SelectableComponent* selectable = entity->GetComponent<SelectableComponent>();
		if (selectable) {
			m_pEntityUnderCursor = entity;
			selectable->HighLightGreen();
		}
	}

	//Turn highligh color back to black on entity if mouse is not over anyomore
	else if (!entity && m_pEntityUnderCursor || entity && entity != m_pEntityUnderCursor) {
		SelectableComponent* selectable = m_pEntityUnderCursor->GetComponent<SelectableComponent>();
		if (selectable->IsSelected()) {
			return;
		}
		selectable->HighLightBlack();
		m_pEntityUnderCursor = nullptr;
	}
}

void PlayerComponent::ExecuteActionbarItem(int32 index)
{
	for (IEntity* entity : m_selectedUnits) {
		SelectableComponent* selectable = entity->GetComponent<SelectableComponent>();
		if (selectable) {
			selectable->GetAllUIItems()[index]->Execute();
		}
		else {
			continue;
		}
	}
}
