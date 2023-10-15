#pragma once

struct SEngineerInfo {
public:
	f32 m_timeBetweenBuilds = 1.f;
	f32 m_maxBuildDistance = 20.f;
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
	IEntity* m_pBuildingAssigned = nullptr;

	SEngineerInfo m_pEngineerInfo;
public:
	void AssignBuilding(IEntity* buildingEntity);
	bool HasBuildingAssigned();
	void CancelBuildingAssigned();

	void SetEngineerInfo(SEngineerInfo engineerInfo);
	SEngineerInfo GetEngineerInfo();
};