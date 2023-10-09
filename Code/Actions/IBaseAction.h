#pragma once

class IBaseAction {

protected:
	IEntity* m_pEntity = nullptr;
	bool bIsDone = false;

public:
	virtual void Execute() = 0;
	virtual void Cancel() = 0;
	virtual bool IsDone() = 0;
};