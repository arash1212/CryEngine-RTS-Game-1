#pragma once

class IBaseInfoPanelUIItem;

class InfoPanelUIDetailComponent final : public IEntityComponent
{

public:
	InfoPanelUIDetailComponent() = default;
	virtual ~InfoPanelUIDetailComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<InfoPanelUIDetailComponent>& desc)
	{
		desc.SetGUID("{3E8BA467-313D-4E73-BE71-CD289F6B5520}"_cry_guid);
		desc.SetEditorCategory("Selectable");
	}

private:
	IBaseInfoPanelUIItem* m_pUIInfoPanelUIItem = nullptr;

public:
	void SetInfoPanelUIItem(IBaseInfoPanelUIItem* infoPanelUIItem);
	IBaseInfoPanelUIItem* GetInfoPanelUIItem();

};
