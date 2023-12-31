#pragma once

struct IUIElement;
struct IUIElementEventListener;

class UIActionbarComponent final : public IEntityComponent
{

public:
	UIActionbarComponent() = default;
	virtual ~UIActionbarComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<UIActionbarComponent>& desc)
	{
		desc.SetGUID("{C632D0A7-7139-4BEC-AAE8-03891A0E6C30}"_cry_guid);
		desc.SetEditorCategory("UI");
	}

private:
	IUIElement* m_pActionbarUIElement = nullptr;;

public:
	void AddButton(string imagePath);
	void Clear();

	void SetEventListener(IUIElementEventListener* eventListener);
};