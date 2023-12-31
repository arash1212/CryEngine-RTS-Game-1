#pragma once

#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include <CryAISystem/Components/IEntityNavigationComponent.h>

class UnitStateManagerComponent;

static constexpr f32 AI_CONTROLLER_DEFAULT_MOVE_SPEED = 0.4f;

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
		desc.AddMember(&AIControllerComponent::m_moveSpeed, 'ms', "movespeed", "Move Speed", "Set Move Speed", AI_CONTROLLER_DEFAULT_MOVE_SPEED);
	}

private:
	Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterControllerComponent = nullptr;
	IEntityNavigationComponent* m_pNavigationComponent = nullptr;
	UnitStateManagerComponent* m_pStateManager = nullptr;

private:
	Vec3 m_moveToPosition = ZERO;
	f32 m_moveSpeed = AI_CONTROLLER_DEFAULT_MOVE_SPEED;

	bool bIsStopped = true;

private:
	void Move(f32 DeltaTime);
	Vec3 SnapToNavmesh(Vec3 point);

	Vec3 GetRandomPointInsideTriangle(Triangle t);

public:
	bool MoveTo(Vec3 position, bool run);
	void LookAt(Vec3 position);
	void LookAtWalkDirection();
	f32 AngleTo(Vec3 position);
	void StopMoving();

	bool IsMoving();
	bool IsOnGround();
	void SetMoveSpeed(f32 speed);

	Vec3 GetVelocity();
	Cry::DefaultComponents::CCharacterControllerComponent* GetCharacterController();

	Vec3 GetRandomPointOnNavmesh(float MaxDistance, Vec3 Around);

	bool IsDestinationReachable(Vec3 position);
};
