#pragma once

#include <DefaultComponents/Cameras/CameraComponent.h>
#include <DefaultComponents/Input/InputComponent.h>

class SelectionBoxComponent;
class ActionbarComponent;
class UIElementEventListener;

static const f32 PLAYER_CAMERA_DEFAULT_HEIGHT = 10.f;
static const f32 PLAYER_CAMERA_DEFAULT_MOVE_SPEED = 6.5f;
static const f32 PLAYER_CAMERA_MAX_ZOOM_AMOUNT = 5.f;

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
	
	SelectionBoxComponent* m_pSelectionBoxComponent;
	ActionbarComponent* m_pActionbarComponent;

	//UI
	UIElementEventListener* m_pUIElementEventListener = nullptr;

private:
	f32 m_cameraDefaultHeight = PLAYER_CAMERA_DEFAULT_HEIGHT;
	f32 m_cameraMoveSpeed = PLAYER_CAMERA_DEFAULT_MOVE_SPEED;
	f32 m_cameraMaxZoomAmount = PLAYER_CAMERA_MAX_ZOOM_AMOUNT;
	f32 m_currentZoomAmount = 0;
	f32 m_defaultPosZ = 0;
	f32 m_movementSpeed = 620.f;
	Vec3 m_movementOffset = ZERO;

	DynArray<IEntity*> m_selectedUnits;


	//Right click count
	int32 m_rightClickCount = 0;
	f32 m_timeBetweenRightClickCountRestart = 0.3f;
	f32 m_rightClickCountRestartTimePassed = 0.0f;

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

	//Actions
	void DeselectUnits();
	void SelectUnits();

	//Commands
	void CommandUnitsToMove(Vec3 position);
	void SetUnitsAttackTarget(IEntity* target); 

	//Action bar
	void AddUIItemsToActionbar();

public:
	void ExecuteActionbarItem(int32 index);
};
