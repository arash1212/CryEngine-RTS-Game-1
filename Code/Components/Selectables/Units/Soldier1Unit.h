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
class AttackerComponent;

struct SOwnerInfo;

class Soldier1UnitComponent final : public IEntityComponent
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
	}


private:
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	SelectableComponent* m_pSelectableComponent = nullptr;
	AIControllerComponent* m_pAIController = nullptr;
	ActionManagerComponent* m_pActionManagerComponent = nullptr;
	BaseWeaponComponent* m_pWeaponComponent = nullptr;
	UnitStateManagerComponent* m_pStateManagerComponent = nullptr;
	AttackerComponent* m_pAttackerComponent = nullptr;

	//OwnerShip
	OwnerInfoComponent* m_pOwnerInfoComponent = nullptr;

private:
	//Animations
	FragmentID m_idleFragmentId;
	FragmentID m_runFragmentId;
	FragmentID m_walkFragmentId;
	FragmentID m_crouchFragmentId;
	FragmentID m_proneFragmentId;
	FragmentID m_attack1FragmentId;
	FragmentID m_activeFragmentId;

protected:
	 void UpdateAnimations();

public:
};