#pragma once

#include <Resources/IResource.h>

class MoneyResource :public IResource {
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
	virtual int32 GetPrice() const override;
	virtual string GetName() const override;
	virtual string GetIcon() const override;
	virtual string GetBuyIcon() const override;
	virtual string GetSellIcon() const override;
	virtual string GetAttachmentName() const override;
	virtual EResourceType GetType() const override;

	virtual SDescription GetDescription() const override {
		SResourceInfo price;
		price.m_moneyAmount = nPrice;

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

static IResource* RESOURCE_MONEY = new MoneyResource();