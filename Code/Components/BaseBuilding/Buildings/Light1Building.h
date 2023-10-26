#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Geometry/StaticMeshComponent.h>
#include <DefaultComponents/Physics/BoxPrimitiveComponent.h>
#include <DefaultComponents/Effects/DecalComponent.h>
#include <DefaultComponents/Lights/PointLightComponent.h>

class BuildingComponent;
class SelectableComponent;
class CostComponent;

struct SResourceInfo;

static string LIGHT_BUILDING_1_MODEL_PATH = "Objects/buildings/light1/light1.cdf";
static string LIGHT_BUILDING_1_TRUSS_MODEL_PATH = "Objects/buildings/light1/truss/light1_truss.cgf";

class Light1BuildingComponent final : public IEntityComponent
{

public:
	Light1BuildingComponent() = default;
	virtual ~Light1BuildingComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<Light1BuildingComponent>& desc)
	{
		desc.SetGUID("{8E0CF8E0-1888-49EB-8926-61C115966E9E}"_cry_guid);
		desc.SetEditorCategory("Building");
	}

private:
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	Cry::DefaultComponents::CStaticMeshComponent* m_pTrussMeshComponent = nullptr;
	Cry::DefaultComponents::CBoxPrimitiveComponent* m_pBboxComponent = nullptr;
	Cry::DefaultComponents::CDecalComponent* m_pDecalComponent = nullptr;
	Cry::DefaultComponents::CPointLightComponent* m_pPointLightComponent = nullptr;

	SelectableComponent* m_pSelectableComponent = nullptr;
	BuildingComponent* m_pBuildingComponent = nullptr;
	CostComponent* m_pCostComponent = nullptr;

	IAttachment* m_pLightPosAttachment = nullptr;
private:

public:
	static SResourceInfo GetCost();
};
