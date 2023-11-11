#pragma once

#include <Resources/IResource.h>

class OilResource : IResource {
private:
	int32 nPrice = 1;
	string sName = "Oil";
	string sDescription = "Oil.";
	string sBuyDescription = "Buy Oil.";
	string sSellDescription = "Sell Oil.";
	string sIcon = "oil_icon_ui.png";
	string sBuyIcon = "oil_barrel_buy_icon.png";
	string sSellIcon = "oil_barrel_sell_icon.png";
	string sAttachmentName = "oilBarrel";
	EResourceType mResourceType = EResourceType::OIL;

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
