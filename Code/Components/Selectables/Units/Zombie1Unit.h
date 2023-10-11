#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>

class SelectableComponent;
class AIControllerComponent;
class IBaseAction;
class ActionManager;
class ActionManagerComponent;
class OwnerInfoComponent;
class UnitStateManagerComponent;
class AttackerComponent;

struct SOwnerInfo;

class Zombie1UnitComponent final : public IEntityComponent
{

public:
	Zombie1UnitComponent() = default;
	virtual ~Zombie1UnitComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<Zombie1UnitComponent>& desc)
	{
		desc.SetGUID("{263AE3B2-702A-4D26-8DF1-8AF91AFF8EE3}"_cry_guid);
		desc.SetEditorCategory("Units");
	}



private:
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	SelectableComponent* m_pSelectableComponent = nullptr;
	AIControllerComponent* m_pAIController = nullptr;
	ActionManagerComponent* m_pActionManagerComponent = nullptr;
	UnitStateManagerComponent* m_pStateManagerComponent = nullptr;
	AttackerComponent* m_pAttackerComponent = nullptr;

	//OwnerShip
	OwnerInfoComponent* m_pOwnerInfoComponent = nullptr;

private:
	//Animations
	FragmentID m_idleFragmentId;
	FragmentID m_runFragmentId;
	FragmentID m_walkFragmentId;
	FragmentID m_attack1FragmentId;
	FragmentID m_attack2FragmentId;
	FragmentID m_attack3FragmentId;
	//FragmentID m_crouchFragmentId;
	//FragmentID m_proneFragmentId;
	FragmentID m_activeFragmentId;
	FragmentID m_testFragmentId;

protected:
	void UpdateAnimations();

public:
};
