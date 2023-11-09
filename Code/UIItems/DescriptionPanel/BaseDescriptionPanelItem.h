#pragma once

class BaseDescriptionPanelItem {
public:
	BaseDescriptionPanelItem(string imagePath, int32 amount);
protected:
	string m_imagePath = "";
	int32 m_amount = 0;
public:
	string GetImagePath();
	int32 GetAmount();
};