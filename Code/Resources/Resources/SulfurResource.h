#pragma once

#include <Resources/IResource.h>

class SulfurResource :public IResource {
private:
	int32 nPrice = 13;
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

static IResource* RESOURCE_SULFUR = new SulfurResource();
