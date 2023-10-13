#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Geometry/StaticMeshComponent.h>
#include <DefaultComponents/Physics/BoxPrimitiveComponent.h>
#include <DefaultComponents/Lights/EnvironmentProbeComponent.h>
#include <DefaultComponents/Effects/DecalComponent.h>

struct SBuildingInfo {
public:
	f32 m_maxBuildAmount = 20.f;
};

class BuildingComponent final : public IEntityComponent
{

public:
	BuildingComponent() = default;
	virtual ~BuildingComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<BuildingComponent>& desc)
	{
		desc.SetGUID("{0C10513B-AC27-44D7-8BE4-9F51FD53FD66}"_cry_guid);
		desc.SetEditorCategory("Building");
	}

private:
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	Cry::DefaultComponents::CStaticMeshComponent* m_pTrussMeshComponent = nullptr;
	Cry::DefaultComponents::CBoxPrimitiveComponent* m_pBboxComponent = nullptr;
	Cry::DefaultComponents::CDecalComponent* m_pDecalComponent = nullptr;
	SBuildingInfo m_pBuildingInfo;

private:
	bool bIsPlaced = false;
	bool bIsBuilt = false;

	f32 m_currentBuiltAmount = 0.f;
	f32 m_timeBetweenBuilds = 1.f;
	f32 m_builtTimePassed = 0.f;

private:
	void UpdateMaterial();

public:
	void Place(Vec3 at);
	void Build();
	void SetBuildingInfo(SBuildingInfo buildingInfo);

	bool IsPlaced();
	bool CanBePlaced();
};