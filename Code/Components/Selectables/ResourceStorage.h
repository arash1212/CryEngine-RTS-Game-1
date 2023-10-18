#pragma once

#include <Components/Managers/ResourceManager.h>

class ResourceStorageComponent final : public IEntityComponent
{

public:
	ResourceStorageComponent() = default;
	virtual ~ResourceStorageComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<ResourceStorageComponent>& desc)
	{
		desc.SetGUID("{BF6F3526-A8D9-4AA5-94EB-6835E240EFE9}"_cry_guid);
		desc.SetEditorCategory("Resource");
	}


private:
	bool bIsOilStorage = false;

public:
	bool IsOilStorage();
};