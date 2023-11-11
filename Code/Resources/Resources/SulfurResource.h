#pragma once

#include <Resources/IResource.h>

class SulfurResource : IResource {
private:
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
