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
class UnitAnimationComponent;
class EngineerComponent;
class CostComponent;
class ResourceCollectorComponent;
class WorkerComponent;
class UnitTypeManagerComponent;

struct SOwnerInfo;
struct SResourceInfo;

class Engineer1UnitComponent final : public IEntityComponent
{

public:
	Engineer1UnitComponent() = default;
	virtual ~Engineer1UnitComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<Engineer1UnitComponent>& desc)
	{
		desc.SetGUID("{0A695E55-3956-420A-B972-6FB32EB76802}"_cry_guid);
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
	EngineerComponent* m_pEngineerComponent = nullptr;
	CostComponent* m_pCostComponent = nullptr;
	ResourceCollectorComponent* m_pResourceCollectorComponent = nullptr;
	WorkerComponent* m_pWorkerComponent = nullptr;
	UnitTypeManagerComponent* m_pUnitTypeManagerComponent = nullptr;

	//OwnerShip
	OwnerInfoComponent* m_pOwnerInfoComponent = nullptr;

private:

public:

	static SResourceInfo GetCost();
};