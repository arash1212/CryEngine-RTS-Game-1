#pragma once

#include <DefaultComponents/Cameras/CameraComponent.h>
#include <DefaultComponents/Input/InputComponent.h>
#include <DefaultComponents/Audio/ListenerComponent.h>

class UISelectionBoxComponent;
class UIActionbarComponent;
class UIElementEventListener;
class BaseBuildingComponent;
class OwnerInfoComponent;
class UIResourcesPanelComponent;
class ResourceManagerComponent;
class UIHealthbarsComponent;
class UIInfoPanelComponent;
class UIDescriptionsPanelComponent;
class IBaseUIItem;
class VisibilityManagerComponent;

enum class EUnitType;

static const f32 PLAYER_CAMERA_DEFAULT_HEIGHT = 25.f;
static const f32 PLAYER_CAMERA_DEFAULT_MOVE_SPEED = 28.f;
static const f32 PLAYER_CAMERA_MAX_ZOOM_AMOUNT = 10.f;
static const string PLAYER_ENTITY_NAME = "PlayerEntity";

class PlayerComponent final : public IEntityComponent
{

public:
	PlayerComponent() = default;
	virtual ~PlayerComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<PlayerComponent>& desc)
	{
		desc.SetGUID("{003322D3-A7FE-4A30-914F-9DC20C20E847}"_cry_guid);
		desc.SetEditorCategory("Player");
		desc.AddMember(&PlayerComponent::m_cameraDefaultHeight, 'pcdh', "cameradefaultheight", "Camera Default Height", "Set Camera Default Height", PLAYER_CAMERA_DEFAULT_HEIGHT);
		desc.AddMember(&PlayerComponent::m_cameraMoveSpeed, 'cms', "cameradmovespeed", "Camera Move Speed", "Set Camera Move Speed", PLAYER_CAMERA_DEFAULT_MOVE_SPEED);
		desc.AddMember(&PlayerComponent::m_cameraMaxZoomAmount, 'cmza', "cameramaxzoomamount", "Camera Max Zoom Amount", "Set Camera Max Zoom Amount", PLAYER_CAMERA_MAX_ZOOM_AMOUNT);
	}

private:
	Cry::DefaultComponents::CCameraComponent* m_pCameraComponent = nullptr;
	Cry::DefaultComponents::CInputComponent* m_pInputComponent = nullptr;
	Cry::Audio::DefaultComponents::CListenerComponent* m_pListenerComp = nullptr;
	IEntityAudioComponent* m_pAudioComponent = nullptr;
	
	UISelectionBoxComponent* m_pUISelectionBoxComponent;
	UIActionbarComponent* m_pUIActionbarComponent;
	BaseBuildingComponent* m_pBaseBuildingComponent = nullptr;
	OwnerInfoComponent* m_pOwnerInfoComponent = nullptr;
	UIResourcesPanelComponent* m_pResouecesPanelComponent = nullptr;
	ResourceManagerComponent* m_pResourceManagerComponent = nullptr;
	UIHealthbarsComponent* m_pHealthbarsComponent = nullptr;
	UIInfoPanelComponent* m_pUIInfoPanelComponent = nullptr;
	UIDescriptionsPanelComponent* m_pUIDescriptionsPanelComponent = nullptr;
	VisibilityManagerComponent* m_pVisibilityManagerComponent = nullptr;

	//UI
	UIElementEventListener* m_pUIElementEventListener = nullptr;

	IEntity* m_pEntityUnderCursor = nullptr;

	DynArray<IBaseUIItem*> m_currentUIItems;

private:
	f32 m_cameraDefaultHeight = PLAYER_CAMERA_DEFAULT_HEIGHT;
	f32 m_cameraMoveSpeed = PLAYER_CAMERA_DEFAULT_MOVE_SPEED;
	f32 m_cameraMaxZoomAmount = PLAYER_CAMERA_MAX_ZOOM_AMOUNT;
	f32 m_currentZoomAmount = 0;
	f32 m_defaultPosZ = 0;
	Vec3 m_movementOffset = ZERO;

	DynArray<IEntity*> m_selectedUnits;


	//Right click count
	int32 m_rightClickCount = 0;
	f32 m_timeBetweenRightClickCountRestart = 0.15f;
	f32 m_rightClickCountRestartTimePassed = 0.0f;
	f32 m_timeBetweenMouseOverCheck = 0.03f;
	f32 m_mouseOverCheckTimePassed = 0.0f;

	int32 m_lastIndecCheck = -1;
	//
	//bool bIsLeftClickWorks = false;

	//
	int32 m_lastSelectablesCheckSize = 0;
	int32 m_lastBuildingActionsCheckSize = 0;
	f32 m_lastMainIconHealthAmount = 0;
private:
	void InitInputs();
	void Move(f32 DeltaTime);

	//Input Handlers;
	void MoveForward(int activationMode, float value);
	void MoveBackward(int activationMode, float value);
	void MoveRight(int activationMode, float value);
	void MoveLeft(int activationMode, float value);
	void MouseWheelDown(int activationMode, float value);
	void MouseWheelUp(int activationMode, float value);
	void LeftMouseDown(int activationMode, float value);
	void RightMouseDown(int activationMode, float value);
	void RotateLeft(int activationMode, float value);
	void RotateRight(int activationMode, float value);

	//Actions
	void DeselectSelectables();
	void SelectSelectables();

	//Commands
	void CommandUnitsToMove(Vec3 position);
	void SetUnitsAttackTarget(IEntity* target); 
	void AssignBuildingToEngineers(IEntity* buildingEntity);
	void AssignResourceToEngineers(IEntity* resourceEntity);
	void AssignWorkplaceToWorkers(IEntity* workplaceEntity);
	void CommandUnitsToSendResourceToWareHouse(IEntity* warehouse);

	//Action bar
	void AddUIItemsToActionbar();

	//
	void CheckSelectablesMouseOver();
	//
	bool AreSelectedUnitsSameType();

	void BoxSelectEntities(Vec2 mousePos);

	void UpdateSelectables();
	int32 CountSelectedUnitType(EUnitType type);

	void UpdateInfoPanel(IEntity* entity);

public:
	void ExecuteActionbarItem(int32 index);
	void ExecuteInfoPanelItem(int32 index);
	void UpdateDescriptionPanel(int32 index);

	void DeselectUnitsOfType(EUnitType type);
};
