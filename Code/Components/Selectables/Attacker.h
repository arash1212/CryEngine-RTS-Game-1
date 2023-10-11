#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>

class OwnerInfoComponent;
class UnitStateManagerComponent;
class AIControllerComponent;
class BaseWeaponComponent;

struct SUnitAttackInfo {
public:
	f32 m_maxAttackDistance = 20.f;
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
	}
private:
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	UnitStateManagerComponent* m_pStateManagerComponent = nullptr;
	AIControllerComponent* m_pAIController = nullptr;
	BaseWeaponComponent* m_pWeaponComponent = nullptr;

	//Target
	IEntity* m_pAttackTargetEntity = nullptr;
	IEntity* m_pRandomAttackTarget = nullptr;

	//AttackInfo
	SUnitAttackInfo m_pAttackInfo;
	DynArray<FragmentID> m_pAttackAnimations;

private:
	bool bIsHumanoid = false;

	bool bUpdatedAnimation = false;

	f32 m_timeBetweenAttacks = 0.04f;
	f32 m_attackTimePassed = 0.f;
	f32 m_timeBetweenAttackCountReset = 0.9f;
	f32 m_attackCountResetTimePassed = 0.f;
	int32 m_maxAttackCount = 10;
	int32 m_attackCount = 0;

public:
	void Attack(IEntity* target);
	void AttackRandomTarget();
	void FindRandomTarget();
	void LookAt(Vec3 position);

	bool IsUpdatedAnimation();
	void SetUpdatedAnimation(bool updatedAnimation);

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

	bool IsAttacking();

	void SetAttackAnimations(DynArray<FragmentID> attackAnimations);
};