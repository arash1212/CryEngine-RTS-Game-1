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
	}

private:
	Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterControllerComponent = nullptr;

	EUnitState m_pUnitState;
	EUnitStance m_pUnitStance;

private:
	f32 m_walkSpeed = 0.f;
	f32 m_runSpeed = 0.f;

private:
	void UpdateState();

public:
	EUnitState GetState();
	void SetStance(EUnitStance stance);

	//
	void SetCharacterController(Cry::DefaultComponents::CCharacterControllerComponent* characterControllerComponent);
	void SetWalkSpeed(f32 walkSpeed);
	void SetRunSpeed(f32 runSpeed);
};