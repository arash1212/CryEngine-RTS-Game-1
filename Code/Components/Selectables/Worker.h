#pragma once

class AIControllerComponent;
class ResourceManagerComponent;
class ResourceCollectorComponent;
class ResourceManagerComponent;
class OwnerInfoComponent;

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
	ResourceCollectorComponent* m_pResourceCollectorComponent = nullptr;
	ResourceManagerComponent* m_pResourceManagerComponent = nullptr;
	OwnerInfoComponent* m_pOwnerInfoComponent = nullptr;

private:
	IEntity* m_pAssignedWorkplace = nullptr;
	bool bIsEnteredWorkplace = false;
	bool bIsReachedAssignedPoint = false;

	//Timers
	f32 m_timeBetweenWorks = 5.f;
	f32 m_workTimePassed = 0.f;

private:
	void EnterAssignedWorkplace();

	SResourceInfo GetResourceRequestParams(EResourceType resourceType, int32 amount);

public:
	void AssignWorkplace(IEntity* workplace);
	IEntity* GetAssignedWorkplace();
	void CancelAssignedWorkplace();

	bool HasEnteredWorkplace();
	void SetHasEnteredWorkplace(bool hasEntered);

	bool HasReachedAssignedPoint();

	bool PickResourceFromWareHouse(EResourceType resourceType, int32 amount);
	bool TransferResourcesToPosition(Vec3 position);
	bool WaitAndPickResources(int32 waitAmount, Vec3 lookAtPos, EResourceType resourceType, int32 amount);
	bool TransferResourcesToWarehouse(EResourceType resourceType, int32 amount);
};
