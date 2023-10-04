#pragma once


struct IUIElement;

class SelectionBoxComponent final : public IEntityComponent
{

public:
	SelectionBoxComponent() = default;
	virtual ~SelectionBoxComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<SelectionBoxComponent>& desc)
	{
		desc.SetGUID("{6944C2C8-8517-4DFF-9B91-8EEE288ED474}"_cry_guid);
		desc.SetEditorCategory("UI");
	}
private:
	IUIElement* m_pSelectionBoxUIElement;
};