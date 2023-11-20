#pragma once

struct IUIElement;
struct IUIElementEventListener;

class IBaseInfoPanelUIItem;

class UIInfoPanelComponent final : public IEntityComponent
{

public:
	UIInfoPanelComponent() = default;
	virtual ~UIInfoPanelComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<UIInfoPanelComponent>& desc)
	{
		desc.SetGUID("{73432857-13C1-4CDF-A746-AF168CB39538}"_cry_guid);
		desc.SetEditorCategory("UI");
	}


private:
	IUIElement* m_pInfoPanelUIElement = nullptr;
	DynArray<IBaseInfoPanelUIItem*> m_items;

public:
	void AddItem(IBaseInfoPanelUIItem* item);
	void Clear();
	void SetCount(int32 index, int32 count);
	void SetMainIcon(string imagePath);
	void SetMainIconText(string text);
	void SetCircleBarProgress(int32 index, f32 amount);
	void SetProgressAmount(int32 index, f32 amount);

	void ExecuteItem(int32 index);

	void SetEventListener(IUIElementEventListener* eventListener);
};
