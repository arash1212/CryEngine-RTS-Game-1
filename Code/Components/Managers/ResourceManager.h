#pragma once

class UIResourcesPanelComponent;

struct SResourceInfo {
public:
	int32 m_moneyAmount = 650;
	int32 m_oilAmount = 350;
};

class ResourceManagerComponent : public IEntityComponent {

public:
	ResourceManagerComponent() = default;
	virtual ~ResourceManagerComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<ResourceManagerComponent>& desc)
	{
		desc.SetGUID("{B9D55102-CD9C-4AE1-AB71-E53AC98643CB}"_cry_guid);
		desc.SetEditorCategory("Managers");
	}

private:
	UIResourcesPanelComponent* m_pResouecesPanelComponent = nullptr;
	SResourceInfo m_pResouceInfo;

public:
	SResourceInfo GetAvailableResourcesInfo();
	bool RequsetResources(SResourceInfo resourceRequestParams);
	void RefundResources(SResourceInfo resourceRequestParams);
};