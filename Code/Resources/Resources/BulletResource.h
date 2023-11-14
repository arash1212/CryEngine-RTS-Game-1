#pragma once

#include <Resources/IResource.h>

class BulletResource :public IResource {
private:
	int32 nPrice = 6;
	string sName = "Bullet";
	string sDescription = "Bullet.";
	string sBuyDescription = "Buy Bullet.";
	string sSellDescription = "Sell Bullet.";
	string sIcon = "bullet_icon_ui.png";
	string sBuyIcon = "bullet_buy_icon.png";
	string sSellIcon = "bullet_sell_icon.png";
	string sAttachmentName = "bullet";
	EResourceType mResourceType = EResourceType::BULLET;

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

static IResource* RESOURCE_BULLET = new BulletResource();