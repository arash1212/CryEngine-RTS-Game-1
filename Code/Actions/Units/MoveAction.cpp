#include "StdAfx.h"
#include "MoveAction.h"
#include "GamePlugin.h"

#include <Components/Selectables/Units/Soldier1Unit.h>
#include <Components/Controller/AIController.h>

MoveAction::MoveAction(IEntity* entity, Vec3 movePosition, bool run)
{
	this->m_pEntity = entity;
	this->m_movePosition = movePosition;
	this->bRun = run;
}

void MoveAction::Execute()
{
	Soldier1UnitComponent* unit = m_pEntity->GetComponent<Soldier1UnitComponent>();
	if (unit) {
		unit->MoveTo(m_movePosition, bRun);
	}
}

void MoveAction::Cancel()
{
	Soldier1UnitComponent* unit = m_pEntity->GetComponent<Soldier1UnitComponent>();
	if (unit) {
		unit->MoveTo(m_pEntity->GetWorldPos(), false);
		bIsDone = true;
	}
}

bool MoveAction::IsDone()
{
	if (!bIsDone) {
		bIsDone = m_pEntity->GetWorldPos().GetDistance(m_movePosition) <= 1;
	}
	return bIsDone;
}
