#pragma once

class OwnerInfoComponent;
class ActionManagerComponent;

struct SEngineerInfo {
public:
	f32 m_timeBetweenBuilds = 0.8f;
	f32 m_timeBetweenCollecting = 1.f;
	f32 m_maxBuildDistance = 1.f;
};

class EngineerComponent final : public IEntityComponent
{

public:
	EngineerComponent() = default;
	virtual ~EngineerComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<EngineerComponent>& desc)
	{
		desc.SetGUID("{4BB61205-630E-4745-B898-EE2538BBD16A}"_cry_guid);
		desc.SetEditorCategory("Units");
	}

private:
	//OwnerShip
	OwnerInfoComponent* m_pOwnerInfoComponent = nullptr;
	ActionManagerComponent* m_pActionManagerComponent = nullptr;

	IEntity* m_pBuildingAssigned = nullptr;

	SEngineerInfo m_pEngineerInfo;

private:
	f32 fTimeBetweenLookingForBuildingToBuild = 1.f;
	f32 fLookingForBuildingToBuildTimePassed = 0.f;

private:
	IEntity* FindClosestUnBuildBuilding();
	void FindBuildingToBuild();

public:
	void AssignBuilding(IEntity* buildingEntity);
	bool HasBuildingAssigned();
	void CancelBuildingAssigned();

	void SetEngineerInfo(SEngineerInfo engineerInfo);
	SEngineerInfo GetEngineerInfo();
};