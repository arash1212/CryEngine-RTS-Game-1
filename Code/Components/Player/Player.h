#pragma once

class PlayerComponent final : public IEntityComponent
{

public:
	PlayerComponent() = default;
	virtual ~PlayerComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<PlayerComponent>& desc)
	{
		desc.SetGUID("{003322D3-A7FE-4A30-914F-9DC20C20E847}"_cry_guid);
	}

};
