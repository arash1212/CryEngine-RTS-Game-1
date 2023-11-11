#pragma once

#include <Resources/IResource.h>

class IronResource : IResource {
private:
	int32 nPrice = 1;
	string sName = "Iron";
	string sDescription = "Iron.";
	string sBuyDescription = "Buy Iron.";
	string sSellDescription = "Sell Iron.";
	string sIcon = "iron_icon_ui.png";
	string sBuyIcon = "iron_buy_icon.png";
	string sSellIcon = "iron_sell_icon.png";
	string sAttachmentName = "iron";
	EResourceType mResourceType = EResourceType::IRON;

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
	}
};