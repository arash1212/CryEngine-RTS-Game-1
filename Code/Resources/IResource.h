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
public:
	virtual int32 GetPrice() const = 0;
	virtual string GetName() const = 0;
	virtual string GetIcon() const = 0;
	virtual string GetBuyIcon() const = 0;
	virtual string GetSellIcon() const = 0;
	virtual EResourceType GetType() const = 0;
	virtual string GetAttachmentName() const = 0;
	virtual SDescription GetDescription() const = 0;
};

