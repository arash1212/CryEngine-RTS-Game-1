#pragma once

#include <Resources/IResource.h>

class IronResource :public IResource {
private:
	int32 nPrice = 2;
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

static IResource* RESOURCE_IRON = new IronResource();