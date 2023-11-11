#pragma once

#include <Components/Managers/ResourceManager.h>

enum class EResourceType {
	Money,
	OIL,
	POPULATION,
	WHEAT,
	FLOUR,
	WOOD,
	BREAD,
	SULFUR,
	GUN_POWDER,
	IRON,
	BULLET,
	AK47
};

class IResource {
	virtual int32 GetPrice() = 0;
	virtual string GetName() = 0;
	virtual string GetIcon() = 0;
	virtual string GetBuyIcon() = 0;
	virtual string GetSellIcon() = 0;
	virtual EResourceType GetType() = 0;
	virtual string GetAttachmentName() = 0;
	virtual SDescription GetDescription() = 0;
};
