#pragma once

class VisibilityManagerComponent : public IEntityComponent {

public:
	VisibilityManagerComponent() = default;
	virtual ~VisibilityManagerComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<VisibilityManagerComponent>& desc)
	{
		desc.SetGUID("{E3A5356B-9387-4F4E-8183-2D648539DEDC}"_cry_guid);
		desc.SetEditorCategory("Managers");
	}

private:
	DynArray<IEntity*> m_hostilePlayers;

private:
	void CheckVisibility();

public:
};
