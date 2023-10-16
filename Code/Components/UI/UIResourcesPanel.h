#pragma once

struct IUIElement;
struct IUIElementEventListener;

class UIResourcesPanelComponent final : public IEntityComponent
{

public:
	UIResourcesPanelComponent() = default;
	virtual ~UIResourcesPanelComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<UIResourcesPanelComponent>& desc)
	{
		desc.SetGUID("{E6AA5A88-F513-4D79-8325-B0B7B978B476}"_cry_guid);
		desc.SetEditorCategory("UI");
	}

private:
	IUIElement* m_pResourcesPanelUIElement;

private:

public:
	void SetEventListener(IUIElementEventListener* eventListener);
	void SetMoneyAmount(int moneyAmount);
	void SetOilAmount(int moneyAmount);
	void UpdatePanel();
};
