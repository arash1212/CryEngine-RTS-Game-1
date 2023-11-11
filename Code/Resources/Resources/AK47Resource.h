#pragma once

#include <Resources/IResource.h>

class AK47Resource : IResource {
private:
	int32 nPrice = 1;
	string sName = "AK47";
	string sDescription = "AK47.";
	string sBuyDescription = "Buy AK47.";
	string sSellDescription = "Sell AK47.";
	string sIcon = "ak47_Icon_ui.png";
	string sBuyIcon = "ak47_buy_icon.png";
	string sSellIcon = "ak47_sell_icon.png";
	string sAttachmentName = "ak47";
	EResourceType mResourceType = EResourceType::AK47;

public:
	virtual int32 GetPrice() override;
	virtual string GetName() override;
	virtual string GetIcon() override;
	virtual string GetBuyIcon() override;
	virtual string GetSellIcon() override;
	virtual EResourceType GetType() override;
	virtual string GetAttachmentName() override;

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
