#pragma once

#include <Resources/IResource.h>

class WheatResource : IResource {
private:
	int32 nPrice = 1;
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
