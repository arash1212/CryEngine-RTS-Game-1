#pragma once

#include <Resources/IResource.h>

class WheatResource :public IResource {
private:
	int32 nPrice = 2;
	string sName = "Wheat";
	string sDescription = "Wheat.";
	string sBuyDescription = "Buy Wheat.";
	string sSellDescription = "Sell Wheat.";
	string sIcon = "wheat_icon_ui.png";
	string sBuyIcon = "wheat_buy_icon.png";
	string sSellIcon = "wheat_sell_icon.png";
	string sAttachmentName = "wheat";
	EResourceType mResourceType = EResourceType::WHEAT;

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

static IResource* RESOURCE_WHEAT = new WheatResource();