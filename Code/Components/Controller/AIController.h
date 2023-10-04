#pragma once

#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include <CryAISystem/Components/IEntityNavigationComponent.h>

static constexpr f32 AI_CONTROLLER_DEFAULT_MOVE_SPEED = 5.f;

class AIControllerComponent final : public IEntityComponent
{

public:
	AIControllerComponent() = default;
	virtual ~AIControllerComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<AIControllerComponent>& desc)
	{
		desc.SetGUID("{FE13B37C-67FA-4E68-B3D6-A1CF731727FC}"_cry_guid);
		desc.SetEditorCategory("Controller");
	}

private:
	Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterControllerComponent = nullptr;
	IEntityNavigationComponent* m_pNavigationComponent = nullptr;

private:
	Vec3 m_moveToPosition = ZERO;
	f32 m_moveSpeed = AI_CONTROLLER_DEFAULT_MOVE_SPEED;

private:
	void Move();

public:
	void MoveTo(Vec3 position);
	bool IsMoving();
	bool IsOnGround();
	void SetMoveSpeed(f32 speed);
	void StopMoving();
	Vec3 GetVelocity();
};