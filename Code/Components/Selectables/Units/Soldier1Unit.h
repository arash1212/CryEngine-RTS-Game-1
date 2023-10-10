#pragma once

#include <Components/Selectables/IUnit.h>
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

static constexpr f32 BASE_UNIT_DEFAULT_PRONE_SPEED = 0.5f;
static constexpr f32 BASE_UNIT_DEFAULT_CROUCH_SPEED = 1.f;
static constexpr f32 BASE_UNIT_DEFAULT_WALK_SPEED = 2.f;
static constexpr f32 BASE_UNIT_DEFAULT_RUN_SPEED = 2.5f;

class Soldier1UnitComponent final : public IUnitComponent
{

public:
	Soldier1UnitComponent() = default;
	virtual ~Soldier1UnitComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<Soldier1UnitComponent>& desc)
	{
		desc.SetGUID("{47E06D6D-0DFE-4605-80D4-BB858719EE2A}"_cry_guid);
		desc.SetEditorCategory("Units");
		desc.AddMember(&Soldier1UnitComponent::m_proneSpeed, 'ps', "pronespeed", "Prone Speed", "Set Prone Speed", BASE_UNIT_DEFAULT_PRONE_SPEED);
		desc.AddMember(&Soldier1UnitComponent::m_walkSpeed, 'ws', "walkspeed", "Walk Speed", "Set Walk Speed", BASE_UNIT_DEFAULT_WALK_SPEED);
		desc.AddMember(&Soldier1UnitComponent::m_crouchSpeed, 'cs', "crouchspeed", "Crouch Speed", "Set Crouch Speed", BASE_UNIT_DEFAULT_CROUCH_SPEED);
		desc.AddMember(&Soldier1UnitComponent::m_runSpeed, 'rs', "runspeed", "Run Speed", "Set Run Speed", BASE_UNIT_DEFAULT_RUN_SPEED);
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
	FragmentID m_crouchFragmentId;
	FragmentID m_proneFragmentId;
	FragmentID m_activeFragmentId;

	//Speed
	f32 m_walkSpeed = BASE_UNIT_DEFAULT_WALK_SPEED;
	f32 m_crouchSpeed = BASE_UNIT_DEFAULT_CROUCH_SPEED;
	f32 m_proneSpeed = BASE_UNIT_DEFAULT_PRONE_SPEED;
	f32 m_runSpeed = BASE_UNIT_DEFAULT_RUN_SPEED;
	f32 m_currentSpeed = BASE_UNIT_DEFAULT_WALK_SPEED;

protected:
	virtual void UpdateAnimations() override;

public:
	virtual void Attack(IEntity* target) override;
	virtual void LookAt(Vec3 position) override;
	virtual void AttackRandomTarget() override;

	//Actions
	virtual void Stop() override;
	virtual void MoveTo(Vec3 position, bool run) override;
	virtual void StopMoving() override;
	virtual void FindRandomTarget() override;

	virtual void SetTargetEntity(IEntity* target) override;
	virtual SUnitAttackInfo GetAttackInfo() override;

	virtual f32 GetCurrentSpeed() override;
	virtual bool IsRunning() override;
};