#pragma once

class IBaseAction {

protected:
	IEntity* m_pEntity;

public:
	virtual void Execute() = 0;
	virtual void Cancel() = 0;
	virtual bool IsDone() = 0;
};