#include "StdAfx.h"
#include "AttackAction.h"
#include "GamePlugin.h"

#include <Components/Selectables/IUnit.h>
#include <Components/Selectables/Units/Soldier1Unit.h>

AttackAction::AttackAction(IEntity* entity, IEntity* target)
{
	this->m_pEntity = entity;
	this->m_pTarget = target;
}

void AttackAction::Execute()
{
	IUnitComponent* unit = m_pEntity->GetComponent<Soldier1UnitComponent>();
	if (unit) {
		f32 distanceToTarget = m_pEntity->GetWorldPos().GetDistance(m_pTarget->GetWorldPos());
		if (distanceToTarget > unit->GetAttackInfo().m_maxAttackDistance) {
			unit->SetTargetEntity(m_pTarget);
			unit->MoveTo(m_pTarget->GetWorldPos(), false);
		}
		else {
			unit->Attack(m_pTarget);
			unit->LookAt(m_pTarget->GetWorldPos());
		}
	}
}

void AttackAction::Cancel()
{
	Soldier1UnitComponent* unit = m_pEntity->GetComponent<Soldier1UnitComponent>();
	if (unit) {
		unit->SetTargetEntity(nullptr);
		unit->MoveTo(m_pEntity->GetWorldPos(), false);
		bIsDone = true;
	}
}

bool AttackAction::IsDone()
{
	return bIsDone;
}