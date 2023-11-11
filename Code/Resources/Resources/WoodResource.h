#pragma once

#include <Resources/IResource.h>

class WoodResource : IResource {
private:
	int32 nPrice = 1;
	string sName = "Wood";
	string sDescription = "Wood.";
	string sBuyDescription = "Buy Wheat.";
	string sSellDescription = "Sell Wheat.";
	string sIcon = "wood_Icon_ui.png";
	string sBuyIcon = "wood_buy_icon.png";
	string sSellIcon = "wood_sell_icon.png";
	string sAttachmentName = "wood";
	EResourceType mResourceType = EResourceType::WOOD;

public:
	virtual int32 GetPrice() override;
	virtual string GetName() override;
	virtual string GetIcon() override;
	virtual string GetBuyIcon() override;
	virtual string GetSellIcon() override;
	virtual string GetAttachmentName() override;
	virtual EResourceType GetType() override;

	virtual SDescription GetDescription() override {
		SResourceInfo cost;
		cost.m_moneyAmount = nPrice;

		SDescription description;
		description.cost = cost;
		description.sIcon = sIcon;
		description.sName = sName;
		description.sDescription = sDescription;
		description.sBuyDescription = sBuyDescription;
		description.sSellDescription = sSellDescription;

		return description;
	};
};
