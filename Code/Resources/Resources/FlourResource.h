#pragma once

#include <Resources/IResource.h>

class FlourResource : IResource {
private:
	int32 nPrice = 1;
	string sName = "Flour";
	string sDescription = "Flour.";
	string sBuyDescription = "Buy Flour.";
	string sSellDescription = "Sell Flour.";
	string sIcon = "flour_icon_ui.png";
	string sBuyIcon = "flour_buy_icon.png";
	string sSellIcon = "flour_sell_icon.png";
	string sAttachmentName = "flour";
	EResourceType mResourceType = EResourceType::FLOUR;

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
