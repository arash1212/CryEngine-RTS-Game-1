#pragma once

struct IUIElement;
struct IUIElementEventListener;

class UIHealthbarsComponent final : public IEntityComponent
{

public:
	UIHealthbarsComponent() = default;
	virtual ~UIHealthbarsComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<UIHealthbarsComponent>& desc)
	{
		desc.SetGUID("{47904F52-98D1-4A5E-9988-7C8EF11EF955}"_cry_guid);
		desc.SetEditorCategory("UI");
	}


private:
	IUIElement* m_pHealthbarUIElement = nullptr;

private:

public:
	void SetEventListener(IUIElementEventListener* eventListener);
	void AddHealthbar(int32 x, int32 y);
};

