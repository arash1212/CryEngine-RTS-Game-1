#pragma once

#include <Resources/IResource.h>

class PopulationResource : IResource {
private:
	int32 nPrice = 1;
	string sName = "Population";
	string sDescription = "Population.";
	string sBuyDescription = "Buy Population.";
	string sSellDescription = "Sell Population.";
	string sIcon = "population_Icon_ui.png";
	string sBuyIcon = "population_buy_icon.png";
	string sSellIcon = "population_sell_icon.png";
	string sAttachmentName = "";
	EResourceType mResourceType = EResourceType::POPULATION;

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
