#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>

class SelectableComponent;
class AIControllerComponent;
class IBaseAction;
class ActionManager;
class ActionManagerComponent;
class BaseWeaponComponent;
class OwnerInfoComponent;
class UnitStateManagerComponent;

struct SOwnerInfo;

struct SUnitAttackInfo {
	f32 m_maxAttackDistance = 20;
};

static constexpr f32 BASE_UNIT_DEFAULT_WALK_SPEED = 2.f;
static constexpr f32 BASE_UNIT_DEFAULT_RUN_SPEED = 4.f;

class BaseUnitComponent final : public IEntityComponent
{

public:
	BaseUnitComponent() = default;
	virtual ~BaseUnitComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<BaseUnitComponent>& desc)
	{
		desc.SetGUID("{FF8A9AD4-6430-4D59-9292-83A5AA6734E6}"_cry_guid);
		desc.SetEditorCategory("Units");
		desc.AddMember(&BaseUnitComponent::m_walkSpeed, 'ws', "walkspeed", "Walk Speed", "Set Walk Speed", BASE_UNIT_DEFAULT_WALK_SPEED);
		desc.AddMember(&BaseUnitComponent::m_runSpeed, 'rs', "runspeed", "Run Speed", "Set Run Speed", BASE_UNIT_DEFAULT_RUN_SPEED);
	}

private:
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	SelectableComponent* m_pSelectableComponent = nullptr;
	AIControllerComponent* m_pAIController = nullptr;
	ActionManagerComponent* m_pActionManagerComponent = nullptr;
	BaseWeaponComponent* m_pWeaponComponent = nullptr;
	UnitStateManagerComponent* m_pStateManagerComponent = nullptr;

	//Target
	IEntity* m_pAttackTargetEntity = nullptr;
	IEntity* m_pRandomAttackTarget = nullptr;

	//AttackInfo
	SUnitAttackInfo m_pAttackInfo;

	//OwnerShip
	OwnerInfoComponent* m_pOwnerInfoComponent = nullptr;

private:
	//Animations
	FragmentID m_idleFragmentId;
	FragmentID m_runFragmentId;
	FragmentID m_walkFragmentId;
	FragmentID m_activeFragmentId;

	//Speed
	f32 m_walkSpeed = BASE_UNIT_DEFAULT_WALK_SPEED;
	f32 m_runSpeed = BASE_UNIT_DEFAULT_RUN_SPEED;
	f32 m_currentSpeed = BASE_UNIT_DEFAULT_WALK_SPEED;

protected:
	virtual void UpdateAnimations();

public:
	virtual void Attack(IEntity* target);
	virtual void LookAt(Vec3 position);
	virtual void AttackRandomTarget();

	//Actions
	void Stop();
	void MoveTo(Vec3 position, bool run);
	void StopMoving();
	void FindRandomTarget();

	void SetTargetEntity(IEntity* target);
	SUnitAttackInfo GetAttackInfo();

	f32 GetCurrentSpeed();
	bool IsRunning();
};