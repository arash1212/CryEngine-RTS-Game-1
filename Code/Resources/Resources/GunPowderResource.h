#pragma once

#include <Resources/IResource.h>

class GunPowderResource :public IResource {
private:
	int32 nPrice = 14;
	string sName = "GunPowder";
	string sDescription = "GunPowder.";
	string sBuyDescription = "Buy GunPowder.";
	string sSellDescription = "Sell GunPowder.";
	string sIcon = "gun_powder_icon_ui.png";
	string sBuyIcon = "gun_powder_buy_icon.png";
	string sSellIcon = "gun_powder_sell_icon.png";
	string sAttachmentName = "gunPowder";
	EResourceType mResourceType = EResourceType::GUN_POWDER;

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

static IResource* RESOURCE_GUN_POWDER = new GunPowderResource();