#pragma once

class BuildingComponent;

struct IAttachment;

class GuardPostComponent final : public IEntityComponent
{

public:
	GuardPostComponent() = default;
	virtual ~GuardPostComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<GuardPostComponent>& desc)
	{
		desc.SetGUID("{6DAB1F5F-B0A2-4E7D-90D7-DA7B1DD6D714}"_cry_guid);
		desc.SetEditorCategory("Resource");
	}

private:
	BuildingComponent* m_pBuildingComponent = nullptr;

private:
	DynArray<IEntity*> m_pGuardEntities;
	IAttachment* m_currentMoveToAttachment = nullptr;
	DynArray<IAttachment*> m_guardPointAttachments;

private:
	int32 m_maxGuardsCount = 4;

private:
	void UpdateGuardaSlotsUI();

public:
	void SetMaxGuardsCount(int32 count);
	int32 GetMaxGuardsCount();
	int32 GetCurrentGuardsCount();

	int32 AssignGuardToPlace(IEntity* worker);
	void RemovedGuardFromWorkplace(int32 index);

	DynArray<IEntity*> GetGuards();

	DynArray<IAttachment*> GetGuardPointAttachments();
	void SetGuardPointAttachments(DynArray<IAttachment*> guardPointAttachments);

	void UpdateGuardPositions();
};
