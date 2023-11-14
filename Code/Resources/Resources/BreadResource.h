#pragma once

#include <Resources/IResource.h>

class BreadResource : public IResource {
private:
	int32 nPrice = 3;
	string sName = "Bread";
	string sDescription = "Bread.";
	string sBuyDescription = "Buy Bread.";
	string sSellDescription = "Sell Bread.";
	string sIcon = "food_icon_ui.png";
	string sBuyIcon = "bread_buy_icon.png";
	string sSellIcon = "bread_sell_icon.png";
	string sAttachmentName = "bread";
	EResourceType mResourceType = EResourceType::BREAD;

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

static IResource* RESOURCE_BREAD = new BreadResource();
