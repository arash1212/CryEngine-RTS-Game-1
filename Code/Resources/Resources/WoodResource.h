#pragma once

#include <Resources/IResource.h>

class WoodResource :public IResource {
private:
	int32 nPrice = 11;
	string sName = "Wood";
	string sDescription = "Wood.";
	string sBuyDescription = "Buy Wheat.";
	string sSellDescription = "Sell Wheat.";
	string sIcon = "wood_Icon_ui.png";
	string sBuyIcon = "wood_buy_icon.png";
	string sSellIcon = "wood_sell_icon.png";
	string sAttachmentName = "wood";
	EResourceType mResourceType = EResourceType::WOOD;

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
	};
};

static IResource* RESOURCE_WOOD = new WoodResource();