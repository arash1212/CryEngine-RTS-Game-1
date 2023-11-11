#pragma once

#include <Resources/IResource.h>

class MoneyResource : IResource {
private:
	int32 nPrice = 1;
	string sName = "Money";
	string sDescription = "Money.";
	string sBuyDescription = "Buy Money.";
	string sSellDescription = "Sell Money.";
	string sIcon = "money_Icon_ui.png";
	string sBuyIcon = "money_buy_icon.png";
	string sSellIcon = "money_sell_icon.pngg";
	string sAttachmentName = "";
	EResourceType mResourceType = EResourceType::Money;

public:
	virtual int32 GetPrice() override;
	virtual string GetName() override;
	virtual string GetIcon() override;
	virtual string GetBuyIcon() override;
	virtual string GetSellIcon() override;
	virtual string GetAttachmentName() override;
	virtual EResourceType GetType() override;

	virtual SDescription GetDescription() override {
		SResourceInfo price;
		price.m_moneyAmount = 1;

		SResourceInfo sellPrice;
		sellPrice.m_sulfurAmount = 1;

		SDescription description;
		description.price = price;
		description.sellPrice = sellPrice;
		description.sName = sName;
		description.sIcon = sIcon;
		description.sBuyIcon = sBuyIcon;
		description.sSellIcon = sSellIcon;
		description.sDescription = sDescription;
		description.sBuyDescription = sBuyDescription;
		description.sSellDescription = sSellDescription;

		return description;
	}
};