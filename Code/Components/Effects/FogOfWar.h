#pragma once

class ResourceManagerComponent;

class FogOfWarComponent final : public IEntityComponent
{

public:
	FogOfWarComponent() = default;
	virtual ~FogOfWarComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<FogOfWarComponent>& desc)
	{
		desc.SetGUID("{41F671B0-B9D8-41C9-B282-8961F876F742}"_cry_guid);
		desc.SetEditorCategory("Effects");
	}

private:
	ResourceManagerComponent* m_pResourceManagerComponent = nullptr;

private:
	int num = 0;

private:

	void Test();
};
