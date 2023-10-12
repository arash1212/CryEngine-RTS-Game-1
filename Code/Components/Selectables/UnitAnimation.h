#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>

class UnitStateManagerComponent;
class AIControllerComponent;
class AttackerComponent;

class UnitAnimationComponent final : public IEntityComponent
{

public:
	UnitAnimationComponent() = default;
	virtual ~UnitAnimationComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<UnitAnimationComponent>& desc)
	{
		desc.SetGUID("{3388A3CC-BDDC-437A-88F0-B1CBF4C76ED6}"_cry_guid);
		desc.SetEditorCategory("Units");
	}

private:
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	UnitStateManagerComponent* m_pStateManagerComponent = nullptr;
	AIControllerComponent* m_pAIController = nullptr;
	AttackerComponent* m_pAttackerComponent = nullptr;


private:
	bool bIsHumaniod = true;

private:
	///////////Animations
	//Humanoid
	FragmentID m_idleFragmentId;
	FragmentID m_runFragmentId;
	FragmentID m_walkFragmentId;
	FragmentID m_attack1FragmentId;
	FragmentID m_attack2FragmentId;
	FragmentID m_attack3FragmentId;
	FragmentID m_crouchFragmentId;
	FragmentID m_proneFragmentId;
	FragmentID m_activeFragmentId;
	FragmentID m_testFragmentId;

	DynArray<FragmentID> m_pAttackAnimations;

private:
	void UpdateAnimations();

public:
	void PlayRandomAttackAnimation();
};