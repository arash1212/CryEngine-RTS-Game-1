#pragma once

class WorkplaceComponent final : public IEntityComponent
{

public:
	WorkplaceComponent() = default;
	virtual ~WorkplaceComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<WorkplaceComponent>& desc)
	{
		desc.SetGUID("{56217FE4-1133-4D3B-A6E1-B84BFFDD78C1}"_cry_guid);
		desc.SetEditorCategory("Resource");
	}
private:
	BuildingComponent* m_pBuildingComponent = nullptr;

private:
	DynArray<IEntity*> m_pWorkerEntities;
	IAttachment* m_currentMoveToAttachment = nullptr;

private:
	int32 m_maxWorkersCount = 1;

public:
	void SetMaxWorkersCount(int32 count);
	int32 GetCurrentWorkersCount();

	int32 AssignWorkerToPlace(IEntity* worker);
	void RemovedWorkerFromWorkplace(int32 index);

	DynArray<IEntity*> GetWorkers();

	void SetMoveToAttachment(IAttachment* currentMoveToAttachment);
	Vec3 GetCurrentMoveToAttachment();
};
