#include "StdAfx.h"
#include "Player.h"
#include "GamePlugin.h"

#include <UIItems/IBaseUIItem.h>
#include <Components/UI/UISelectionBox.h>
#include <Components/UI/UIActionbar.h>
#include <Components/UI/Listener/UIElementEventListener.h>
#include <Components/UI/UIHealthbars.h>

#include <Utils/MouseUtils.h>
#include <Utils/EntityUtils.h>

#include <Components/Selectables/Selectable.h>
#include <Components/Selectables/Engineer.h>
#include <Components/Selectables/Health.h>

#include <Components/Selectables/Units/Engineer1Unit.h>
#include <Components/Selectables/Units/Soldier1Unit.h>
#include <Components/Selectables/Units/Zombie1Unit.h>
#include <Components/Managers/ActionManager.h>
#include <Actions/Units/UnitMoveAction.h>
#include <Actions/Units/UnitAttackAction.h>
#include <Actions/Units/UnitBuildAction.h>
#include <Components/Info/OwnerInfo.h>
#include <Components/BaseBuilding/Building.h>
#include <Components/Managers/ResourceManager.h>
#include <Actions/Units/UnitCollectResourceAction.h>
#include <Components/Selectables/ResourceCollector.h>
#include <Components/Selectables/Worker.h>
#include <Components/Selectables/Workplace.h>
#include <Actions/Units/UnitSendResourceToWarehouseAction.h>
#include <Components/Selectables/ResourceStorage.h>
#include <Components/BaseBuilding/BaseBuilding.h>
#include <Components/UI/UIResourcesPanel.h>
#include <Components/Managers/UnitTypeManager.h>
#include <Actions/Units/UnitWorkInWorkplaceAction.h>
#include <Components/Selectables/GuardPost.h>
#include <Components/Selectables/Guard.h>
#include <Actions/Units/UnitGuardInGuardPostAction.h>

#include <Components/UI/UIInfoPanel.h>

#include <Components/UI/UIDescriptionsPanel.h>
#include <UIItems/DescriptionPanel/BaseDescriptionPanelItem.h>

#include <UIItems/InfoPanel/IBaseInfoPanelUIItem.h>
#include <UIItems/InfoPanel/Items/UIActionInfoPanelItem.h>
#include <Components/Effects/FogOfWar.h>

#include <Components/Managers/VisibilityManager.h>

#include <Interfaces/IUIInfoPanelItemProvider.h>
#include <Components/Selectables/Units/BaseUnit.h>

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
	m_pCameraComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), Quat::CreateRotationXYZ(Ang3(DEG2RAD(-55), 0, DEG2RAD(0))), Vec3(0, 0, m_cameraDefaultHeight)));

	//Inputs initialization
	m_pInputComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();
	InitInputs();

	m_defaultPosZ = m_pEntity->GetPos().z;

	//UIElementEventListener initialization
	m_pUIElementEventListener = new UIElementEventListener(this);

	//SelectionBox component initialization
	m_pUISelectionBoxComponent = m_pEntity->GetOrCreateComponent<UISelectionBoxComponent>();
	m_pUISelectionBoxComponent->SetCameraComponent(m_pCameraComponent);
	m_pUISelectionBoxComponent->SetEventListener(m_pUIElementEventListener);

	//Actionbar component initialization
	m_pUIActionbarComponent = m_pEntity->GetOrCreateComponent<UIActionbarComponent>();
	m_pUIActionbarComponent->SetEventListener(m_pUIElementEventListener);

	//BaseBuildingComponent initialization
	m_pBaseBuildingComponent = m_pEntity->GetOrCreateComponent<BaseBuildingComponent>();

	//ListenerComponent initialization
	m_pListenerComp = m_pEntity->GetOrCreateComponent<Cry::Audio::DefaultComponents::CListenerComponent>();

	//AudioComponent initialization
	m_pAudioComponent = m_pEntity->GetOrCreateComponent<IEntityAudioComponent>();

	//OwnerInfoComponent initialization
	m_pOwnerInfoComponent = m_pEntity->GetOrCreateComponent<OwnerInfoComponent>();
	m_pOwnerInfoComponent->SetCanBeTarget(false);
	m_pOwnerInfoComponent->SetPlayer(EPlayer::PLAYER1);
	m_pOwnerInfoComponent->SetTeam(EPlayerTeam::TEAM1);
	m_pOwnerInfoComponent->SetFaction(EPlayerFaction::FACTION1);

	//ResouecesPanelComponent initialization
	m_pResouecesPanelComponent = m_pEntity->GetOrCreateComponent<UIResourcesPanelComponent>();
	m_pResouecesPanelComponent->SetEventListener(m_pUIElementEventListener);

	//ResourceManagerComponent initialization
	m_pResourceManagerComponent = m_pEntity->GetOrCreateComponent<ResourceManagerComponent>();
	m_pResourceManagerComponent->SetIsPlayer(true);

	//Update Resource Panel
	m_pResouecesPanelComponent->UpdatePanel();

	//Set player entity name
	m_pEntity->SetName(PLAYER_ENTITY_NAME);

	//HealthbarsComponent initialization
	//m_pHealthbarsComponent = m_pEntity->GetOrCreateComponent<UIHealthbarsComponent>();
//	m_pHealthbarsComponent->SetEventListener(m_pUIElementEventListener);
	
	gEnv->p3DEngine->GetTimeOfDay();

	//UIInfoPanelComponent initialization
	m_pUIInfoPanelComponent = m_pEntity->GetOrCreateComponent<UIInfoPanelComponent>();
	m_pUIInfoPanelComponent->SetEventListener(m_pUIElementEventListener);

	//UIDescriptionsPanelComponent initialization
	m_pUIDescriptionsPanelComponent = m_pEntity->GetOrCreateComponent<UIDescriptionsPanelComponent>();
	m_pUIDescriptionsPanelComponent->SetEventListener(m_pUIElementEventListener);

	//VisibilityManagerComponent initialization
	m_pVisibilityManagerComponent = m_pEntity->GetOrCreateComponent<VisibilityManagerComponent>();

	//FogOfWarComponent initialization
	m_pFogOfWarComponent = m_pEntity->GetOrCreateComponent<FogOfWarComponent>();
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

		UpdateSelectables();

		if (m_rightClickCountRestartTimePassed < m_timeBetweenRightClickCountRestart) {
			m_rightClickCountRestartTimePassed += 0.5f * DeltaTime;
		}
		else {
			m_rightClickCount = 0;
		}

		if (m_mouseOverCheckTimePassed < m_timeBetweenMouseOverCheck) {
			m_mouseOverCheckTimePassed += 0.5f * DeltaTime;
		}
		if (m_descriptionUpdateTimePassed < m_timeBetweenUpdatingDescription) {
			
		}

		if (!m_pUIElementEventListener->IsMouseOverUI()) {
			m_pUIDescriptionsPanelComponent->Clear();
			m_lastActionbarMouseOverIndex = -1;
		}
		else {
			if (m_descriptionUpdateTimePassed < m_timeBetweenUpdatingDescription) {
				m_descriptionUpdateTimePassed += 0.5f * DeltaTime;
			}
			else {
				UpdateDescriptionPanel(m_lastActionbarMouseOverIndex);
				m_descriptionUpdateTimePassed = 0;
			}
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
	Vec3 pos = currentPos + (m_movementOffset) * m_cameraMoveSpeed * DeltaTime;

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

	//RotateLeft
	m_pInputComponent->RegisterAction("player", "rotateLeft", [this](int activationMode, float value) {this->RotateLeft(activationMode, value); });
	m_pInputComponent->BindAction("player", "rotateLeft", eAID_KeyboardMouse, eKI_Q);

	//RotateRight
	m_pInputComponent->RegisterAction("player", "rotateRight", [this](int activationMode, float value) {this->RotateRight(activationMode, value); });
	m_pInputComponent->BindAction("player", "rotateRight", eAID_KeyboardMouse, eKI_E);
}

void PlayerComponent::MoveForward(int activationMode, float value)
{
	m_movementOffset.y = value;
}

void PlayerComponent::MoveBackward(int activationMode, float value)
{
	m_movementOffset.y = -value;
}

void PlayerComponent::MoveRight(int activationMode, float value)
{
	m_movementOffset.x = value;
}

void PlayerComponent::MoveLeft(int activationMode, float value)
{
	m_movementOffset.x = -value;
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
		m_pUISelectionBoxComponent->SetBoxInitPosition(mousePos);
	}

	if (activationMode == eAAM_OnRelease) {
		if (m_pUIElementEventListener->IsMouseOverUI()) {
			return;
		}
		//TODO :
		//if (!bIsLeftClickWorks) {
		//	bIsLeftClickWorks = true;
		//	return;
		//}

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
		if (entity && !m_pUISelectionBoxComponent->IsBoxSelectionTriggered(mousePos)) {
			OwnerInfoComponent* pOwnerComponent = entity->GetComponent<OwnerInfoComponent>();
			BuildingComponent* pBuildingComponent = entity->GetComponent<BuildingComponent>();
			if (!pOwnerComponent || pOwnerComponent->GetPlayer() != m_pOwnerInfoComponent->GetPlayer() || pBuildingComponent && !pBuildingComponent->IsBuilt()) {
				return;
			}
			m_selectedUnits.push_back(entity);
			SelectSelectables();
		}

		//Box/Multiple Selection
		else if (m_pUISelectionBoxComponent->IsBoxSelectionTriggered(mousePos)) {
			BoxSelectEntities(mousePos);
			SelectSelectables();
		}
		//////////////////////////

		//Actionbar
		//AddUIItemsToActionbar();
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
			BuildingComponent* pBuildingComponent = entity->GetComponent<BuildingComponent>();
			if (pBuildingComponent && !pBuildingComponent->IsBuilt()) {
				AssignBuildingToEngineers(entity);
				return;
			}

			WorkplaceComponent* pWorkplaceComponent = entity->GetComponent<WorkplaceComponent>();
			if (pBuildingComponent && pBuildingComponent->IsBuilt() && pWorkplaceComponent) {
				AssignWorkplaceToWorkers(entity);
				return;
			}

			GuardPostComponent* pGuardPostComponent = entity->GetComponent<GuardPostComponent>();
			if (pBuildingComponent && pBuildingComponent->IsBuilt() && pGuardPostComponent) {
				AssignGuardPostToGuards(entity);
				return;
			}

			ResourceStorageComponent* resourceStorage = entity->GetComponent<ResourceStorageComponent>();
			if (pBuildingComponent && pBuildingComponent->IsBuilt() && resourceStorage) {
				CommandUnitsToSendResourceToWareHouse(entity);
				return;
			}

			BaseResourcePointComponent* resource = entity->GetComponent<BaseResourcePointComponent>();
			if (resource) {
				AssignResourceToEngineers(entity);
				return;
			}
		}
		else {
			CommandUnitsToMove(mousePos);
		}
		///////////////////////////
	}
}

void PlayerComponent::RotateLeft(int activationMode, float value)
{
	if (activationMode == eAAM_OnRelease) {
		if (!m_pBaseBuildingComponent || !m_pBaseBuildingComponent->HasBuildingAssigned()) {
			return;
		}

		m_pBaseBuildingComponent->RotateBuildingToLeft();
	}
}

void PlayerComponent::RotateRight(int activationMode, float value)
{
	if (activationMode == eAAM_OnRelease) {
		if (!m_pBaseBuildingComponent || !m_pBaseBuildingComponent->HasBuildingAssigned()) {
			return;
		}

		m_pBaseBuildingComponent->RotateBuildingToRight();
	}
}

/*=============================================================================================================================================
																	ACTIONS
==============================================================================================================================================*/

void PlayerComponent::DeselectSelectables()
{
	m_lastMainIconHealthAmount = -1;
	m_lastBuildingActionsCheckSize = -1;
	m_lastSelectablesCheckSize = -1;
	m_lastTypesSizeCheck = -1;
	m_pUIInfoPanelComponent->Clear();
	m_currentUIItems.clear();

	for (IEntity* entity : m_selectedUnits) {
		if (!entity) {
			continue;
		}

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
	for (int32 i = 0; i < m_selectedUnits.size(); i++) {
		IEntity* entity = m_selectedUnits[i];
		if (!entity) {
			continue;
		}

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
	int32 unitsCount = m_selectedUnits.size();
	for (int32 i = 0; i < unitsCount; i++) {
		if (!m_selectedUnits[i]) {
			continue;
		}

		IEntity* closestUnit = EntityUtils::GetClosestEntity(m_selectedUnits, position);
		ActionManagerComponent* actionManager = m_selectedUnits[i]->GetComponent<ActionManagerComponent>();
		if (actionManager) {
			f32 diffX = crymath::abs(position.x - closestUnit->GetWorldPos().x);
			f32 diffY = crymath::abs(position.y - closestUnit->GetWorldPos().y);
			int32 rowSize = unitsCount / 2;
			if (unitsCount < 4) {
				rowSize = 2;
			}

			if (diffX > diffY) {
				//int32 temp = row;
				//row = column;
				//column = temp;

				if (i != 0) {
					row++;
				}
				if (i != 0 && i % rowSize == 0) {
					column++;
					row = 0;
				}
			}
			else {
				if (i != 0) {
					column++;
				}
				if (i != 0 && i % rowSize == 0) {
					row++;
					column = 0;
				}
			}

			AABB aabb;
			m_selectedUnits[i]->GetWorldBounds(aabb);
			f32 width = aabb.max.x - aabb.min.x;
			f32 height = aabb.max.y - aabb.min.y;
			Vec3 pos = Vec3(position.x + ((column * (width + 1.0f))), position.y - ((row * (height + 1.0f))), position.z);

			IPersistantDebug* pd = gEnv->pGameFramework->GetIPersistantDebug();
			if (pd) {
				pd->Begin("moveCommand" + i, true);
				pd->AddSphere(pos, 0.3f, ColorF(0, 1, 0), 1);
			}
			actionManager->AddAction(new UnitMoveAction(m_selectedUnits[i], pos, m_rightClickCount >= 2));
		}
		else {
			continue;
		}
	}
}

void PlayerComponent::SetUnitsAttackTarget(IEntity* target)
{
	for (IEntity* entity : m_selectedUnits) {
		if (!entity) {
			continue;
		}

		ActionManagerComponent* actionManager = entity->GetComponent<ActionManagerComponent>();
		if (actionManager) {
			if (target) {
				actionManager->AddAction(new UnitAttackAction(entity, target));
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
		if (!entity) {
			continue;
		}

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

void PlayerComponent::AssignResourceToEngineers(IEntity* resourceEntity)
{
	for (IEntity* entity : m_selectedUnits) {
		if (!entity) {
			continue;
		}

		if (!entity->GetComponent<EngineerComponent>()) {
			continue;
		}
		if (!entity->GetComponent<ResourceCollectorComponent>()) {
			continue;
		}
		ActionManagerComponent* actionManager = entity->GetComponent<ActionManagerComponent>();
		if (actionManager) {
			if (resourceEntity) {
				actionManager->AddAction(new UnitCollectResourceAction(entity, resourceEntity));
				CryLog("Resource assigned to collector.");
			}
		}
		else {
			continue;
		}
	}
}

void PlayerComponent::AssignWorkplaceToWorkers(IEntity* workplaceEntity)
{
	for (IEntity* entity : m_selectedUnits) {
		if (!entity) {
			continue;
		}

		WorkerComponent* workerComponent = entity->GetComponent<WorkerComponent>();
		if (!workerComponent) {
			continue;
		}
		WorkplaceComponent* pWorkPlaceComponent = workplaceEntity->GetComponent<WorkplaceComponent>();
		if (!pWorkPlaceComponent) {
			return;
		}
		ActionManagerComponent* pActionManagerComponent = entity->GetComponent<ActionManagerComponent>();
		if (pActionManagerComponent) {
			if (workplaceEntity) {
				pActionManagerComponent->AddAction(new UnitWorkInWorkplaceAction(entity, workplaceEntity));
				CryLog("workplace assigned %s:", workplaceEntity->GetName());
			}
		}
		else {
			continue;
		}
	}
}

void PlayerComponent::AssignGuardPostToGuards(IEntity* guardPosteEntity)
{
	for (IEntity* entity : m_selectedUnits) {
		if (!entity) {
			continue;
		}

		GuardComponent* pGuardComponent = entity->GetComponent<GuardComponent>();
		if (!pGuardComponent) {
			continue;
		}
		GuardPostComponent* pGuardPostComponent = guardPosteEntity->GetComponent<GuardPostComponent>();
		if (!pGuardPostComponent) {
			return;
		}
		ActionManagerComponent* pActionManagerComponent = entity->GetComponent<ActionManagerComponent>();
		if (pActionManagerComponent) {
			if (pGuardPostComponent) {
				pActionManagerComponent->AddAction(new UnitGuardInGuardPostAction(entity, guardPosteEntity));
				CryLog("guardPost assigned %s:", guardPosteEntity->GetName());
			}
		}
		else {
			continue;
		}
	}
}

void PlayerComponent::CommandUnitsToSendResourceToWareHouse(IEntity* warehouse)
{
	for (IEntity* entity : m_selectedUnits) {
		if (!entity) {
			continue;
		}

		if (!entity->GetComponent<ResourceCollectorComponent>()) {
			continue;
		}
		ActionManagerComponent* actionManager = entity->GetComponent<ActionManagerComponent>();
		if (actionManager) {
			if (warehouse) {
				actionManager->AddAction(new UnitSendResourceToWarehouseAction(entity, warehouse));
			}
		}
		else {
			continue;
		}
	}
}

void PlayerComponent::AddUIItemsToActionbar()
{
	m_pUIActionbarComponent->Clear();
	if (m_selectedUnits.size() <= 0) {
		return;
	}

	SelectableComponent* selectable = m_selectedUnits[0]->GetComponent<SelectableComponent>();
	if (m_selectedUnits.size() > 1 && !AreSelectedUnitsSameType()) {
		m_currentUIItems = selectable->GetGeneralUIItems();
	}

	if (m_selectedUnits.size() == 1 || AreSelectedUnitsSameType()) {
		m_currentUIItems = selectable->GetAllUIItems();
	}

	if (selectable) {
		for (IBaseUIItem* uiItem : m_currentUIItems) {
			m_pUIActionbarComponent->AddButton(uiItem->GetImagePath());
		}
	}
}

void PlayerComponent::CheckSelectablesMouseOver()
{
	//if (m_mouseOverCheckTimePassed >= m_timeBetweenMouseOverCheck) {
		//Turn highligh color to green on entity if it's selectable
		IEntity* entity = MouseUtils::GetActorUnderCursor();
		if (!entity && !m_pEntityUnderCursor) {
			return;
		}

		if (entity) {
			SelectableComponent* selectable = entity->GetComponent<SelectableComponent>();
			if (selectable) {
				if (selectable->IsSelected()) {
					return;
				}

				if (m_pEntityUnderCursor && !m_pEntityUnderCursor->IsGarbage() && m_pEntityUnderCursor != entity) {
					SelectableComponent* oldSelectable = m_pEntityUnderCursor->GetComponent<SelectableComponent>();
					oldSelectable->HighLightBlack();
				}
				m_pEntityUnderCursor = entity;

				if (m_pOwnerInfoComponent->IsEntityHostile(entity)) {
					selectable->HighLightRed();
				}
				else {
					selectable->HighLightGreen();
				}
				return;
			}
		}

		//Turn highligh color back to black on entity if mouse is not over anyomore
		else if (m_pEntityUnderCursor) {
			SelectableComponent* selectable = m_pEntityUnderCursor->GetComponent<SelectableComponent>();
			if (selectable->IsSelected()) {
				return;
			}
			selectable->HighLightBlack();
			m_pEntityUnderCursor = nullptr;
		}

		//m_mouseOverCheckTimePassed = 0;
	//}
}

void PlayerComponent::ExecuteActionbarItem(int32 index)
{
	//bIsLeftClickWorks = false;
	for (IEntity* entity : m_selectedUnits) {
		SelectableComponent* selectable = entity->GetComponent<SelectableComponent>();
		if (!selectable) {
			continue;
		}

		if (m_selectedUnits.size() > 1 && !AreSelectedUnitsSameType()) {
			m_currentUIItems[index]->Execute();
			continue;
		}

		if (m_selectedUnits.size() == 1 || AreSelectedUnitsSameType()) {
			m_currentUIItems = selectable->GetAllUIItems();
			m_currentUIItems[index]->Execute();
			//break;
		}
	}
}

void PlayerComponent::ExecuteInfoPanelItem(int32 index)
{
	//bIsLeftClickWorks = false;
	m_pUIInfoPanelComponent->ExecuteItem(index);
}

void PlayerComponent::UpdateDescriptionPanel(int32 index)
{
	bool bShouldUpdateIcons = false;
	if (m_lastActionbarMouseOverIndex != index) {
		bShouldUpdateIcons = true;
	}

	m_lastActionbarMouseOverIndex = index;
	if (m_lastActionbarMouseOverIndex == -1) {
		return;
	}

	IBaseUIItem* pUIItem = m_currentUIItems[m_lastActionbarMouseOverIndex];
	if (!pUIItem) {
		return;
	}
	SDescription pDescription = pUIItem->GetDescrption();
	if (bShouldUpdateIcons) {
		m_pUIDescriptionsPanelComponent->AddDescription(pDescription);
	}
	else {
		m_pUIDescriptionsPanelComponent->UpdateDescriptions();
	}
}

bool PlayerComponent::AreSelectedUnitsSameType()
{
	UnitTypeManagerComponent* pUnitTypeManager = m_selectedUnits[0]->GetComponent<UnitTypeManagerComponent>();
	if (!pUnitTypeManager) {
		return false;
	}
	EUnitType type = pUnitTypeManager->GetUnitType();

	for (IEntity* entity : m_selectedUnits) {
		UnitTypeManagerComponent* unitTypeManager = entity->GetComponent<UnitTypeManagerComponent>();
		if (!unitTypeManager) {
			return false;
		}
		if (unitTypeManager->GetUnitType() != type) {
			return false;
		}
	}

	return true;
}

void PlayerComponent::BoxSelectEntities(Vec2 mousePos)
{
	DynArray<IEntity*> entities;
	DynArray<IEntity*> result;
	entities = m_pUISelectionBoxComponent->GetEntitiesInsideBox(mousePos);
	for (IEntity* entity : entities) {
		OwnerInfoComponent* pOwnerComponent = entity->GetComponent<OwnerInfoComponent>();
		if (!pOwnerComponent || pOwnerComponent->GetPlayer() != m_pOwnerInfoComponent->GetPlayer()) {
			continue;
		}

		result.append(entity);
	}

	m_selectedUnits = result;
}

void PlayerComponent::UpdateSelectables()
{
	if (m_selectedUnits.size() != m_lastSelectablesCheckSize) {
		m_lastSelectablesCheckSize = m_selectedUnits.size();
		m_lastTypesSizeCheck = -1;
		m_pUIInfoPanelComponent->Clear();
		AddUIItemsToActionbar();
		SelectSelectables();
	}

	DynArray<IEntity*> result;
	for (IEntity* entity : m_selectedUnits) {
		if (!entity || entity->IsGarbage()) {
			continue;
		}

		result.append(entity);
	}
	m_selectedUnits = result;

	UpdateInfoPanel();
}

int32 PlayerComponent::CountSelectedUnitType(EUnitType type)
{
	int32 count = 0;
	for (IEntity* entity : m_selectedUnits) {
		if (!entity) {
			continue;
		}
		UnitTypeManagerComponent* pUnitTypeManagerComponent = entity->GetComponent< UnitTypeManagerComponent>();
		if (!pUnitTypeManagerComponent) {
			continue;
		}
		if (pUnitTypeManagerComponent->GetUnitType() == type) {
			count++;
		}
	}
	return count;
}

void PlayerComponent::UpdateInfoPanel()
{
	//InfoPanel (Units)*****************************************************************************************************
	DynArray<EUnitType> types;
	DynArray<IBaseInfoPanelUIItem*> items;
	for (int32 i = 0; i < m_selectedUnits.size(); i++) {
		IEntity* entity = m_selectedUnits[i];
		UnitTypeManagerComponent* pUnitTypeManagerComponent = entity->GetComponent<UnitTypeManagerComponent>();
		if (!pUnitTypeManagerComponent) {
			continue;
		}
		IUIInfoPanelItemProvider* pInfoPanelItemProvider = entity->GetComponent<BaseUnitComponent>();
		if (!pInfoPanelItemProvider) {
			return;
		}

		EUnitType type = pUnitTypeManagerComponent->GetUnitType();
		if (types.empty()) {
			types.append(type);
			items.append(pInfoPanelItemProvider->GetInfoPanelItem());
		}
		else {
			bool bShouldAddNewItem = false;
			for (int32 i = 0; i < types.size(); i++) {
				if (types[i] != type) {
					bShouldAddNewItem = true;
				}
				else {
					bShouldAddNewItem = false;
				}
			}
			if (bShouldAddNewItem) {
				types.append(type);
				items.append(pInfoPanelItemProvider->GetInfoPanelItem());
			}
		}
	}
	if (m_lastTypesSizeCheck != types.size()) {
		for (int32 j = 0; j < types.size(); j++) {
			m_pUIInfoPanelComponent->AddItem(items[j]);
			m_pUIInfoPanelComponent->SetCount(j, CountSelectedUnitType(types[j]));
		}
		m_lastTypesSizeCheck = types.size();
	}

	//Buildings infoPanelItems (Actions))***********************************************************************************
	if (m_selectedUnits.size() == 1) {
		IEntity* entity = m_selectedUnits[0];
		BuildingComponent* pBuildingComponent = entity->GetComponent<BuildingComponent>();
		if (!pBuildingComponent) {
			return;
		}
		//Main icon
		HealthComponent* pHealthComponent = entity->GetComponent<HealthComponent>();
		ActionManagerComponent* pActionManagerComponet = entity->GetComponent<ActionManagerComponent>();
		std::deque<IBaseAction*> queue = pActionManagerComponet->GetActionsQueue();

		//TODO : agar ziad anjam behse error : pure function call zaman clear mide 
		if (m_lastBuildingActionsCheckSize != pActionManagerComponet->GetActiveActionsCount()) {
			m_pUIInfoPanelComponent->Clear();
			m_pUIInfoPanelComponent->SetMainIcon(pBuildingComponent->GetImagePath());
			m_lastMainIconHealthAmount = -1;
			for (IBaseAction* action : queue) {
				if (action->IsDone()) {
					continue;
				}

				IUIInfoPanelItemProvider* pInfoPanelItemProvider = action;
				m_pUIInfoPanelComponent->AddItem(pInfoPanelItemProvider->GetInfoPanelItem());
			}
			m_lastBuildingActionsCheckSize = pActionManagerComponet->GetActiveActionsCount();
		}

		//update action progress
		for (int32 i = 0; i < pActionManagerComponet->GetActionsQueue().size(); i++) {
			m_pUIInfoPanelComponent->SetCount(i, (int)pActionManagerComponet->GetActionsQueue()[i]->GetProgressAmount());
			m_pUIInfoPanelComponent->SetProgressAmount(i, pActionManagerComponet->GetActionsQueue()[i]->GetProgressAmount() / pActionManagerComponet->GetActionsQueue()[i]->GetMaxProgressAmount());
		}

		if (m_lastMainIconHealthAmount != pHealthComponent->GetCurrentHealth()) {
			m_lastMainIconHealthAmount = pHealthComponent->GetCurrentHealth();
			string text = "";
			text.AppendFormat("%6.0lf / %6.0lf", crymath::floor(pHealthComponent->GetCurrentHealth()), crymath::floor(pHealthComponent->GetMaxHealth()));
			m_pUIInfoPanelComponent->SetMainIconText(text);
		}
	}
}

void PlayerComponent::DeselectUnitsOfType(EUnitType type)
{
	DynArray<IEntity*> result;
	for (IEntity* entity : m_selectedUnits) {
		if (!entity) {
			continue;
		}
		UnitTypeManagerComponent* pUnitTypeManagerComponent = entity->GetComponent< UnitTypeManagerComponent>();
		if (!pUnitTypeManagerComponent) {
			continue;
		}
		if (pUnitTypeManagerComponent->GetUnitType() != type) {
			result.append(entity);
		}
	}
	DeselectSelectables();
	m_selectedUnits = result;
	SelectSelectables();
}
