#pragma once

#include <Components/Managers/ResourceManager.h>

class IBaseUIItem {

protected:
	IEntity* m_pEntity = nullptr;

public:
	virtual void Execute() = 0;
	virtual string GetImagePath() = 0;

	virtual SDescription GetDescrption() {
		SResourceInfo price;

		SDescription description;
		description.price = price;

		return description;
	}
};