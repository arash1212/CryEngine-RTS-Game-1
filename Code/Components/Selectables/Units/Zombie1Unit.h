#pragma once

#include <Components/Selectables/Units/BaseUnit.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>

class SelectableComponent;
class AIControllerComponent;
class IBaseAction;
class ActionManager;
class ActionManagerComponent;
class OwnerInfoComponent;
class UnitStateManagerComponent;
class AttackerComponent;
class UnitAnimationComponent;
class CostComponent;
class UnitTypeManagerComponent;
class HealthComponent;
class VisibilityComponent;


struct SOwnerInfo;
struct SResourceInfo;
struct SDescription;

class Zombie1UnitComponent final : public BaseUnitComponent
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
	UnitAnimationComponent* m_pUnitAnimationComponent = nullptr;
	CostComponent* m_pCostComponent = nullptr;
	UnitTypeManagerComponent* m_pUnitTypeManagerComponent = nullptr;
	HealthComponent* m_pHealthComponent = nullptr;
	VisibilityComponent* m_pVisibilityComponent = nullptr;

	//OwnerShip
	OwnerInfoComponent* m_pOwnerInfoComponent = nullptr;

private:

public:
	static SDescription GetDescription();
	virtual IBaseInfoPanelUIItem* GetInfoPanelItem() override;
};
