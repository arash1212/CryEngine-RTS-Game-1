#pragma once

class UIResourcesPanelComponent;
enum class EResourceType;

struct SResourceInfo {
public:
	int32 m_moneyAmount = 650;
	int32 m_oilAmount = 350;
	int32 m_populationUsed = 0;
	int32 m_populationAmount = 20;
	int32 m_wheatAmount = 0;
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
	IEntityAudioComponent* m_pAudioComponent = nullptr;
	SResourceInfo m_pResouceInfo;

	DynArray<IEntity*> m_pOwnedEntities;

public:
	const static int32 m_oilPrice = 10;
	const static int32 m_WheatPrice = 5;

	//Sounds
	CryAudio::ControlId m_pBuySound;
	CryAudio::ControlId m_pSellSound;
private:
	void UpdatePopulation();

public:
	SResourceInfo GetAvailableResourcesInfo();
	bool RequsetResources(SResourceInfo resourceRequestParams);
	void RefundResources(SResourceInfo resourceRequestParams);
	void AddResource(EResourceType type, int32 amount);

	void AddOwnedEntity(IEntity* unit);
	void RemoveOwnedEntity(IEntity* unit);

	void SellResource(int32 amount, EResourceType type);
	void BuyResource(int32 amount, EResourceType type);
};