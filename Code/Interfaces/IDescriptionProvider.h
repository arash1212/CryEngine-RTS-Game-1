#pragma once

#include <Components/Managers/ResourceManager.h>

class IDescriptonProvider {
	virtual SDescription GetDescription() = 0;
};