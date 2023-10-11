#pragma once

#include <DefaultComponents/Physics/CharacterControllerComponent.h>

enum class EUnitState {
	IDLE,
	RUN,
	WALK,
	PRONE
};

enum class EUnitStance {
	STANDING,
	CROUCH,
	PRONE
};

static constexpr f32 BASE_DEFAULT_PRONE_SPEED = 1.0f;
static constexpr f32 BASE_DEFAULT_CROUCH_SPEED = 3.f;
static constexpr f32 BASE_DEFAULT_WALK_SPEED = 5.f;
static constexpr f32 BASE_DEFAULT_RUN_SPEED = 6.5f;

class UnitStateManagerComponent final : public IEntityComponent
{

public:
	UnitStateManagerComponent() = default;
	virtual ~UnitStateManagerComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<UnitStateManagerComponent>& desc)
	{
		desc.SetGUID("{CBEBDDFF-B8F4-4FF3-BC42-EEB4D2AC7400}"_cry_guid);
		desc.SetEditorCategory("Managers");
		desc.AddMember(&UnitStateManagerComponent::m_proneSpeed, 'ps', "pronespeed", "Prone Speed", "Set Prone Speed", BASE_DEFAULT_PRONE_SPEED);
		desc.AddMember(&UnitStateManagerComponent::m_walkSpeed, 'ws', "walkspeed", "Walk Speed", "Set Walk Speed", BASE_DEFAULT_WALK_SPEED);
		desc.AddMember(&UnitStateManagerComponent::m_crouchSpeed, 'cs', "crouchspeed", "Crouch Speed", "Set Crouch Speed", BASE_DEFAULT_CROUCH_SPEED);
		desc.AddMember(&UnitStateManagerComponent::m_runSpeed, 'rs', "runspeed", "Run Speed", "Set Run Speed", BASE_DEFAULT_RUN_SPEED);
	}

private:
	Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterControllerComponent = nullptr;

	EUnitState m_pUnitState;
	EUnitStance m_pUnitStance;

private:
	f32 m_proneSpeed = BASE_DEFAULT_PRONE_SPEED;
	f32 m_crouchSpeed = BASE_DEFAULT_CROUCH_SPEED;
	f32 m_walkSpeed = BASE_DEFAULT_WALK_SPEED;
	f32 m_runSpeed = BASE_DEFAULT_RUN_SPEED;
	f32 m_currentSpeed = BASE_DEFAULT_WALK_SPEED;

private:
	void UpdateState();
	void UpdateCurrentSpeed();

public:
	EUnitState GetState();
	void SetState(EUnitState state);
	EUnitStance GetStance();
	void SetStance(EUnitStance stance);

	//
	void SetCharacterController(Cry::DefaultComponents::CCharacterControllerComponent* characterControllerComponent);
	void SetWalkSpeed(f32 walkSpeed);
	f32 GetWalkSpeed();
	void SetCrouchSpeed(f32 crouchSpeed);
	void SetRunSpeed(f32 runSpeed);
	f32 GetRunSpeed();
	void SetProneSpeed(f32 proneSpeed);

	void SetCurrentSpeed(f32 currentSpeed);
	f32 GetCurrentSpeed();
	bool IsRunning();
};