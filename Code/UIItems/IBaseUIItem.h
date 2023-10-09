#pragma once

class IBaseUIItem {

protected:
	IEntity* m_pEntity = nullptr;

public:
	virtual void Execute() = 0;
	virtual string GetImagePath() = 0;
};