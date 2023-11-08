#pragma once

class IBaseAction;

class ActionManagerComponent : public IEntityComponent {

public:
	ActionManagerComponent() = default;
	virtual ~ActionManagerComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<ActionManagerComponent>& desc)
	{
		desc.SetGUID("{46472D1E-5011-4CAC-9E72-FA1CC73ED44A}"_cry_guid);
		desc.SetEditorCategory("Managers");
	}

protected:
	std::deque<IBaseAction*> m_actionsQueue;
	IBaseAction* m_pCurrentAction;
	bool bIsBuilding = false;

public:
	void ProcessActions();
	void AddAction(IBaseAction* action);
	void CancelCurrentAction();
	bool IsProcessingAnAction();
	IBaseAction* GetCurrentAction();
	std::deque<IBaseAction*> GetActionsQueue();

	bool IsBuilding();
	void SetIsBuilding(bool isBuilding);

	int32 GetActiveActionsCount();
};