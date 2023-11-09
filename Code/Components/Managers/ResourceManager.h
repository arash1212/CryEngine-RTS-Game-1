#pragma once

class UIResourcesPanelComponent;
enum class EResourceType;

struct SResourceInfo {
public:
	int32 m_moneyAmount = 0;
	int32 m_oilAmount = 0;
	int32 m_populationUsed = 0;
	int32 m_populationAmount = 0;
	int32 m_wheatAmount = 0;
	int32 m_flourAmount = 0;
	int32 m_woodAmount = 0;
	int32 m_breadAmount = 0;
	int32 m_sulfurAmount = 0;
	int32 m_gunPowderAmount = 0;
	int32 m_ironAmount = 0;
	int32 m_bulletAmount = 0;
	int32 m_ak47Amount = 0;
};

struct SResourceRequestParams {
	EResourceType m_pResourceType;
	int32 m_amountRequested = 0;
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

private:
	int32 m_lastPopulationCheclAmount = 0;
public:
	const static int32 m_oilPrice = 2;
	const static int32 m_WheatPrice = 2;
	const static int32 m_FlourPrice = 4;
	const static int32 m_WoodPrice = 1;
	const static int32 m_BreadPrice = 3;
	const static int32 m_SulfurPrice = 4;
	const static int32 m_GunPowderPrice = 4;
	const static int32 m_IronPrice = 2;
	const static int32 m_bulletPrice = 5;
	const static int32 m_ak47Price = 10;

	//Sounds
	CryAudio::ControlId m_pBuySound;
	CryAudio::ControlId m_pSellSound;

	bool bIsPlayer = false;
	bool bIsinitDone = false;

	//Timers
	f32 m_timeBetweenEatingFoods = 50.f;
	f32 m_eatingTimePassed = 0.f;
private:
	void UpdatePopulation();

	void CheckFood();
public:
	SResourceInfo GetAvailableResourcesInfo();
	bool RequsetResources(SResourceInfo resourceRequestParams);
	bool CheckIfResourcesAvailable(SResourceInfo resourceRequestParams);
	void RefundResources(SResourceInfo resourceRequestParams);
	void AddResource(EResourceType type, int32 amount);

	void AddOwnedEntity(IEntity* unit);
	void RemoveOwnedEntity(IEntity* unit);

	void SellResource(int32 amount, EResourceType type);
	void BuyResource(int32 amount, EResourceType type);

	void SetIsPlayer(bool isPlayer);
	bool IsPlayer();

	DynArray<IEntity*> GetOwnedEntities();
};