#pragma once

#include "FlashUI/FlashUI.h"

class PlayerComponent;

struct IUIElement;

class UIElementEventListener : public IUIElementEventListener {
public:
	UIElementEventListener(PlayerComponent* playerComponent);

private:
	PlayerComponent* m_pPlayerComponent = nullptr;

private:
	bool bIsMouseOverUI = false;
	IUIElement* m_pLastMouseOverUIEventSender = nullptr;

public:
	virtual void OnUIEvent(IUIElement* pSender, const SUIEventDesc& event, const SUIArguments& args) override;

	bool IsMouseOverUI();
};