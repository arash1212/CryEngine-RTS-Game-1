#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>

class OwnerInfoComponent;
class UnitStateManagerComponent;
class AIControllerComponent;
class BaseWeaponComponent;
class UnitAnimationComponent;
class ActionManagerComponent;

enum class EAttackType {
	RANGED,
	MELEE
};

struct SUnitAttackInfo {
public:
	f32 m_maxAttackDistance = 20.f;
	f32 m_detectionDistance = 30.f; //Used to find "RandomAttackTarget"
	bool bIsHumanoid = false;
	bool bIsFollower = true; //Follows "RandomAttackTarget" or not if it's not in this unit's attack range.
	EAttackType m_pAttackType = EAttackType::RANGED;
	f32 m_timeBetweenAttacks = 0.04f;
};

static constexpr f32 DEFAULT_DAMAGE_AMOUNT = 20.F;

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
		/*
		desc.AddMember(&AttackerComponent::bIsHumanoid, 'ish', "ishumanoid", "Is Humanoid", "Set if is Humanoid", false);
		desc.AddMember(&AttackerComponent::bIsRanged, 'isr', "isranged", "Is Ranged", "Set if is Ranged", false);
		desc.AddMember(&AttackerComponent::bIsFollower, 'isf', "isfollower", "Is Follower", "Set if is Follower (follows random target ?)", false);
		*/
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
	f32 m_damageAmount = DEFAULT_DAMAGE_AMOUNT;

	//Timers
	f32 m_attackTimePassed = 0.f;
	f32 m_timeBetweenAttackCountReset = 0.9f;
	f32 m_attackCountResetTimePassed = 0.f;

	//Attack Count
	int32 m_maxAttackCount = 10;
	int32 m_attackCount = 0;

	DynArray<IEntity*> m_hostilePlayers;

	bool bIsCheckedForHstilePlayers = false;
private:
	void FindRandomTarget();
	void AttackRandomTarget();
	void ValidateTarget();

	//Attack types
	void PerformMeleeAttack(IEntity* target);
	void PerformRangedAttack(IEntity* target);

	void ApplyDamageToTarget(IEntity* target);

	void FindHostilePlayers();

public:
	void Attack(IEntity* target);
	void LookAt(Vec3 position);

	bool IsAttacking();
	bool CanAttack();

	SUnitAttackInfo GetAttackInfo();
	void SetAttackInfo(SUnitAttackInfo attackInfo);

	void SetTargetEntity(IEntity* target);

	void SetDamageAmount(f32 damage);
	f32 GetDamageAmount();
};