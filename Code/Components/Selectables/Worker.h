#pragma once

class AIControllerComponent;

class WorkerComponent final : public IEntityComponent
{

public:
	WorkerComponent() = default;
	virtual ~WorkerComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<WorkerComponent>& desc)
	{
		desc.SetGUID("{A3087BD9-8A53-4CB1-896F-DDA787DAE602}"_cry_guid);
		desc.SetEditorCategory("Units");
	}

private:
	AIControllerComponent* m_pAIController = nullptr;
private:
	IEntity* m_pAssignedWorkplace = nullptr;
	bool bIsEnteredWorkplace = false;
	bool bIsReachedAssignedPoint = false;

private:
	void EnterAssignedWorkplace();

public:
	void AssignWorkplace(IEntity* workplace);
	void CancelAssignedWorkplace();

	bool HasEnteredWorkplace();
	void SetHasEnteredWorkplace(bool hasEntered);

	bool HasReachedAssignedPoint();
};
