#pragma once

#include <Resources/IResource.h>

class PopulationResource :public IResource {
private:
	int32 nPrice = 1;
	string sName = "Population";
	string sDescription = "Population.";
	string sBuyDescription = "Buy Population.";
	string sSellDescription = "Sell Population.";
	string sIcon = "population_Icon_ui.png";
	string sBuyIcon = "population_Icon_ui.png";
	string sSellIcon = "population_Icon_ui.png";
	string sAttachmentName = "";
	EResourceType mResourceType = EResourceType::POPULATION;

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

static IResource* RESOURCE_POPULATION = new PopulationResource();