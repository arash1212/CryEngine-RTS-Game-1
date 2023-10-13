#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>

class OwnerInfoComponent;
class UnitStateManagerComponent;
class AIControllerComponent;
class BaseWeaponComponent;
class UnitAnimationComponent;
class ActionManagerComponent;

struct SUnitAttackInfo {
public:
	f32 m_maxAttackDistance = 20.f;
	f32 m_detectionDistance = 30.f;
};

class AttackerComponent final : public IEntityComponent
{

public:
	AttackerComponent() = default;
	virtual ~AttackerComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<AttackerComponent>& desc)
	{
		desc.SetGUID("{3DB1B862-DB6E-4BA5-AB7A-097245E5080B}"_cry_guid);
		desc.SetEditorCategory("Units");
		desc.AddMember(&AttackerComponent::bIsHumanoid, 'ish', "ishumanoid", "Is Humanoid", "Set if is Humanoid", false);
		desc.AddMember(&AttackerComponent::bIsRanged, 'isr', "isranged", "Is Ranged", "Set if is Ranged", false);
		desc.AddMember(&AttackerComponent::bIsFollower, 'isf', "isfollower", "Is Follower", "Set if is Follower (follows random target ?)", false);
	}

private:
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	UnitStateManagerComponent* m_pStateManagerComponent = nullptr;
	AIControllerComponent* m_pAIController = nullptr;
	BaseWeaponComponent* m_pWeaponComponent = nullptr;
	UnitAnimationComponent* m_pUnitAnimationComponent = nullptr;
	ActionManagerComponent* m_pActionManagerComponent = nullptr;

	//Target
	IEntity* m_pAttackTargetEntity = nullptr;
	IEntity* m_pRandomAttackTarget = nullptr;

	//AttackInfo
	SUnitAttackInfo m_pAttackInfo;

private:
	bool bIsHumanoid = false;
	bool bIsRanged = false;
	bool bIsFollower = true;

	//Timers
	f32 m_timeBetweenAttacks = 0.04f;
	f32 m_attackTimePassed = 0.f;
	f32 m_timeBetweenAttackCountReset = 0.9f;
	f32 m_attackCountResetTimePassed = 0.f;

	//Attack Count
	int32 m_maxAttackCount = 10;
	int32 m_attackCount = 0;

private:
	void FindRandomTarget();
	void AttackRandomTarget();

public:
	void Attack(IEntity* target);
	void LookAt(Vec3 position);

	bool IsAttacking();
	bool CanAttack();

	SUnitAttackInfo GetAttackInfo();
	void SetAttackInfo(SUnitAttackInfo attackInfo);

	void SetTargetEntity(IEntity* target);

	IEntity* GetAttackTarget();
	void SetAttackTarget(IEntity* attacktTarget);

	IEntity* GetRandomAttackTarget();
	void SetRandomAttackTarget(IEntity* randomTarget);

	void SetIsHumanoid(bool isHumanoid);
	bool IsHumanoid();

	void SetTimeBetweenAttack(f32 timeBetweenAttacks);

	void SetIsRanged(bool isRanged);
	bool IsRanged();

	void SetIsFollower(bool isFollower);
	bool IsFollower();
};