#pragma once

#include <Resources/IResource.h>

class OilResource :public IResource {
private:
	int32 nPrice = 12;
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

static IResource* RESOURCE_OIL = new OilResource();