#pragma once

#include <Resources/IResource.h>

class GunPowderResource : IResource {
private:
	int32 nPrice = 1;
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

