#pragma once

#include <Components/Managers/ResourceManager.h>

struct SUIItemDescription {
	SUIItemDescription(SResourceInfo cost, string description) {
		this->cost = cost;
		this->m_description = description;
	}

	SResourceInfo cost;
	string m_description = "";
};

class IBaseUIItem {

protected:
	IEntity* m_pEntity = nullptr;

public:
	virtual void Execute() = 0;
	virtual string GetImagePath() = 0;
	virtual SUIItemDescription GetDescrption() {
		SResourceInfo cost;
		return SUIItemDescription(cost, "PlaceHolder");
	}
};