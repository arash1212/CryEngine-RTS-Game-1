#pragma once

enum class EUnitType {
	ENGINEER1,
	SOLDIER1,
	ZOMBIE1
};

class UnitTypeManagerComponent : public IEntityComponent {

public:
	UnitTypeManagerComponent() = default;
	virtual ~UnitTypeManagerComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<UnitTypeManagerComponent>& desc)
	{
		desc.SetGUID("{5D92AF70-1687-4727-B7FB-A999420BCEDF}"_cry_guid);
		desc.SetEditorCategory("Managers");
	}

private:
	EUnitType m_pUnitType;

public:
	EUnitType GetUnitType();
	void SetUnitType(EUnitType type);
};
