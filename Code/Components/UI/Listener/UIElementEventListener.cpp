#include "StdAfx.h"
#include "UIElementEventListener.h"
#include "GamePlugin.h"

#include <Components/Player/Player.h>

#include <FlashUI/FlashUIElement.h>
#include <CryEntitySystem/IEntitySystem.h>

UIElementEventListener::UIElementEventListener(PlayerComponent* playerComponent)
{
	this->m_pPlayerComponent = playerComponent;
}

void UIElementEventListener::OnUIEvent(IUIElement* pSender, const SUIEventDesc& event, const SUIArguments& args)
{
	string eventName = event.sDisplayName;
	if (pSender) {
		if (eventName == "actionbar_button_click") {
			int index = -1; 
			args.GetArg(0).GetValueWithConversion(index);
			m_pPlayerComponent->ExecuteActionbarItem(index);
		}
		else if (eventName == "actionbar_button_mouse_over") {
			bIsMouseOverUI = true;
			int index = -1;
			args.GetArg(0).GetValueWithConversion(index);
			m_pPlayerComponent->UpdateDescriptionPanel(index);
		}
		else if (eventName == "info_panel_button_click") {
			int index = -1;
			args.GetArg(0).GetValueWithConversion(index);
			CryLog("info index clicked %i ", index);
			m_pPlayerComponent->ExecuteInfoPanelItem(index);
		}

		if (eventName == "mouseOverUI") {
			bIsMouseOverUI = true;
			m_pLastMouseOverUIEventSender = pSender;
			return;
		}
		else if (eventName == "mouseNotOverUI" && m_pLastMouseOverUIEventSender && pSender == m_pLastMouseOverUIEventSender) {
			m_pLastMouseOverUIEventSender = nullptr;
			bIsMouseOverUI = false;
		}
	}

}

bool UIElementEventListener::IsMouseOverUI()
{
	return bIsMouseOverUI;
}
