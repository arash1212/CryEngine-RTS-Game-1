#pragma once

class BaseBuildingComponent final : public IEntityComponent
{

public:
	BaseBuildingComponent() = default;
	virtual ~BaseBuildingComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<BaseBuildingComponent>& desc)
	{
		desc.SetGUID("{25F28CD8-AC55-4252-B2EA-8E5055BB999C}"_cry_guid);
		desc.SetEditorCategory("Building");
	}

private:
	IEntityAudioComponent* m_pAudioComponent = nullptr;
	IEntity* m_pBuildingEntity = nullptr;

private:
	//Sounds
	CryAudio::ControlId m_buildingPlacementSound;

private:
	void UpdateBuildingPosition();

public:
	IEntity* AssignBuilding();
	void PlaceBuilding(Vec3 at);
};