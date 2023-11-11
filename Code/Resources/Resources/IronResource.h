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