#pragma once

struct IUIElement;
struct IUIElementEventListener;

class BaseDescriptionPanelItem;

class UIDescriptionsPanelComponent final : public IEntityComponent
{

public:
	UIDescriptionsPanelComponent() = default;
	virtual ~UIDescriptionsPanelComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<UIDescriptionsPanelComponent>& desc)
	{
		desc.SetGUID("{F4DA1E45-DC62-43FA-90DC-8C36D37843A4}"_cry_guid);
		desc.SetEditorCategory("UI");
	}


private:
	IUIElement* m_pDescriptionPanelUIElement = nullptr;
	DynArray<BaseDescriptionPanelItem*> m_items;


public:
	void AddItem(BaseDescriptionPanelItem* item);
	void Clear();
	void SetAmount(int32 index, int32 amount);
	void SetDescriptionText(string text);

	void SetEventListener(IUIElementEventListener* eventListener);
};

