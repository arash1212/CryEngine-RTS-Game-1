#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Geometry/StaticMeshComponent.h>
#include <DefaultComponents/Physics/BoxPrimitiveComponent.h>
#include <DefaultComponents/Lights/EnvironmentProbeComponent.h>
#include <DefaultComponents/Effects/DecalComponent.h>

class ActionManagerComponent;
class SelectableComponent;
class IBaseUIItem;
class OwnerInfoComponent;
class HealthComponent;
class VisibilityComponent;
class CostComponent;

struct SBuildingInfo {
public:
	f32 m_maxBuildAmount = 10.f;
	int32 m_populationProduces = 0;
};

static const string BUILDING_EXIT_POINT_ATTACHMENT_NAME = "exitPoint";
static const string BUILDING_PLACEMENT_GREEN_MATERIAL = "Materials/buildings/building_placement_green_material.mtl";
static const string BUILDING_PLACEMENT_RED_MATERIAL = "Materials/buildings/building_placement_red_material.mtl";
static const string BUILDING_PLACEMENT_GREEN_DECAL_MATERIAL = "Materials/buildings/building_placement_green_material.mtl";
static const string BUILDING_PLACEMENT_RED_DECAL_MATERIAL = "Materials/buildings/building_placement_red_material.mtl";

class BuildingComponent : public IEntityComponent
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

protected:
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	Cry::DefaultComponents::CStaticMeshComponent* m_pTrussMeshComponent = nullptr;
	Cry::DefaultComponents::CBoxPrimitiveComponent* m_pBboxComponent = nullptr;
	Cry::DefaultComponents::CDecalComponent* m_pDecalComponent = nullptr;

	ActionManagerComponent* m_pActionManagerComponent = nullptr;
	SelectableComponent* m_pSelectableComponent = nullptr;
	OwnerInfoComponent* m_pOwnerInfoComponent = nullptr;
	CostComponent* m_pCostComponent = nullptr;
	HealthComponent* m_pHealthComponent = nullptr;
	VisibilityComponent* m_pVisibilityComponent = nullptr;

	SBuildingInfo m_pBuildingInfo;

	IAttachment* m_pSkinAttachment = nullptr;
	IMaterial* m_pDefaultMaterial = nullptr;

	DynArray<IEntity*> m_builders;
private:
	bool bIsPlaced = false;
	bool bIsBuilt = false;
	bool bIsHouse = false;

	f32 m_currentBuiltAmount = 0.f;

	string m_pathToTrussMesh = "";

	//UI Actions
	DynArray< IBaseUIItem*> m_pAllUIItems;

	f32 m_maxHealth = 100.f;

	string m_imagePath = "cancel.png";

protected:
	void UpdateMaterial();
	void RotateSelectionDecal();

public:
	virtual void Place(Vec3 at);
	void Build();
	void SetBuildingInfo(SBuildingInfo buildingInfo);
	SBuildingInfo GetBuildingInfos();

	bool IsPlaced();
	bool CanBePlaced();

	bool IsBuilt();
	void SetBuilt();

	void SetPathToTrussMesh(string path);

	void AddUIItem(IBaseUIItem* item);

	Vec3 GetExitPoint();

	void SetIsHouse(bool IsHouse);
	bool IsHouse();

	bool IsWorkplace();

	void SetMaxHealth(f32 maxHealth);

	f32 GetCurrentBuildAmount();
	f32 GetMaxBuildAmount();

	void SetImagePath(string imagePath);
	string GetImagePath();

	void AddBuilder(IEntity* builderEntity);
	void RemoveBuilder(IEntity* builderEntity);
	DynArray<IEntity*> GetBuilders();
};