#pragma once

#include <Components/Managers/ResourceManager.h>

class IBaseUIItem {

protected:
	IEntity* m_pEntity = nullptr;

protected:

public:
	virtual void Execute() = 0;

	//TODO : bashe chun har item momkene ye chiz khast be onvan image bekhad
	virtual string GetImagePath() = 0;

	virtual SDescription GetDescrption() {
		SResourceInfo price;

		SDescription description;
		description.price = price;

		return description;
	}
};