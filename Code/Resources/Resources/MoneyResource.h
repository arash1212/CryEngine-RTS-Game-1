#pragma once

#include <Resources/IResource.h>

class MoneyResource : IResource {
private:
	int32 nPrice = 1;
	string sName = "Money";
	string sDescription = "Money.";
	string sBuyDescription = "Buy Money.";
	string sSellDescription = "Sell Money.";
	string sIcon = "money_Icon_ui.png";
	string sBuyIcon = "cancel.png";
	string sSellIcon = "cancel.png";
	string sAttachmentName = "";
	EResourceType mResourceType = EResourceType::Money;

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