#pragma once

#include "StdAfx.h"

struct SUnitAttackInfo {
public:
	f32 m_maxAttackDistance = 20.f;
};

class IUnitComponent : public IEntityComponent
{

public:
	IUnitComponent() = default;
	virtual ~IUnitComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;


	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<IUnitComponent>& desc)
	{
		desc.SetGUID("{597764A6-FC8E-4440-AC25-2A4AAC2FFBD0}"_cry_guid);
		desc.SetEditorCategory("Units");
	}

public:
	void Hello() {
		CryLog("helloololol");
	}

protected:
	virtual void UpdateAnimations();

public:
	virtual void Attack(IEntity* target);
	virtual void LookAt(Vec3 position);
	virtual void AttackRandomTarget();

	//Actions
	virtual void Stop();
	virtual void MoveTo(Vec3 position, bool run);
	virtual void StopMoving();
	virtual void FindRandomTarget();

	virtual void SetTargetEntity(IEntity* target);
	virtual SUnitAttackInfo GetAttackInfo();

	virtual f32 GetCurrentSpeed();
	virtual bool IsRunning();
};