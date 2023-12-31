#pragma once

#include "DefaultComponents/Effects/DecalComponent.h"

class IBaseUIItem;

static const string SELECTION_UNIT_DECAL_MATERIAL = "Materials/decals/selection/unit_selection_decal_material_1.mtl";

class SelectableComponent final : public IEntityComponent
{

public:
	SelectableComponent() = default;
	virtual ~SelectableComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<SelectableComponent>& desc)
	{
		desc.SetGUID("{1A76CDAA-E5DE-4397-BBCC-78E718A3EB4A}"_cry_guid);
		desc.SetEditorCategory("Units");
		desc.AddMember(&SelectableComponent::bIsSelected, 'iss', "isselected", "Is Selected", "Set if is selected", false);
	}

private:
	Cry::DefaultComponents::CDecalComponent* m_pSelectionDecalComponent = nullptr;
	IRenderNode* m_pRenderNode = nullptr;

private:
	bool bIsSelected = false;;
	bool bIsBuilding = false;

	//Highligh Colors
	uint32  m_redColor = (uint32)(int_round(4 * 255.0f) << 24) | (int_round(1 * 255.0f) << 26) | (int_round(0 * 255.0f) << 8) | (int_round(1 * 255.0f));
	uint32  m_greenColor = (uint32)(int_round(0 * 255.0f) << 24) | (int_round(1 * 255.0f) << 16) | (int_round(0 * 255.0f) << 8) | (int_round(1 * 255.0f));
	uint32  m_blackColor = 0;

	//UI Actions
	DynArray< IBaseUIItem*> m_pUnitSpecificUIItems;
	DynArray< IBaseUIItem*> m_pGeneralUIItems;

private:

public:
	void Select();
	void DeSelect();
	bool IsSelected();

	void SetIsBuilding(bool isBuilding);
	bool IsBuilding();

	void AddUIItem(IBaseUIItem* item);
	void AddGeneralUIItem(IBaseUIItem* item);

	DynArray< IBaseUIItem*> GetAllUIItems();
	DynArray< IBaseUIItem*> GetGeneralUIItems();

	void SetDecalSize(Vec3 size);

	void HighLightGreen();
	void HighLightRed();
	void HighLightBlack();

	void MouseOver();

	void RotateSelectionDecal();
};
