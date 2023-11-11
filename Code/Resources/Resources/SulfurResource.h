#pragma once

#include <Resources/IResource.h>

class SulfurResource : IResource {
private:
	int32 nPrice = 1;
	string sName = "Sulfur";
	string sDescription = "Sulfur.";
	string sBuyDescription = "Buy Sulfur.";
	string sSellDescription = "Sell Sulfur.";
	string sIcon = "sulfur_Icon_ui.png";
	string sBuyIcon = "sulfur_buy_icon.png";
	string sSellIcon = "sulfur_sell_icon.png";
	string sAttachmentName = "sulfur";
	EResourceType mResourceType = EResourceType::SULFUR;

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
