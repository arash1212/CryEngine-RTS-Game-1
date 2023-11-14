#pragma once

#include <Resources/IResource.h>

class AK47Resource :public IResource {
private:
	int32 nPrice = 10;
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
	virtual int32 GetPrice() const override;
	virtual string GetName() const override;
	virtual string GetIcon() const override;
	virtual string GetBuyIcon() const override;
	virtual string GetSellIcon() const override;
	virtual EResourceType GetType() const override;
	virtual string GetAttachmentName() const override;

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

static IResource* RESOURCE_AK47 = new AK47Resource();