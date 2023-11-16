#pragma once

#include <Components/Managers/ResourceManager.h>

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
	ResourceManagerComponent* m_pResourceManagerComponent = nullptr;
	IUIElement* m_pDescriptionPanelUIElement = nullptr;
	DynArray<BaseDescriptionPanelItem*> m_items;


public:
	void AddItem(BaseDescriptionPanelItem* item);
	void Clear();
	void SetText(int32 index, string text, bool isRed);
	void SetDescriptionText(string text);

	void AddDescription(SDescription description);
	void UpdateDescriptions();

	void SetEventListener(IUIElementEventListener* eventListener);

	DynArray<BaseDescriptionPanelItem*> GetItems();
};

