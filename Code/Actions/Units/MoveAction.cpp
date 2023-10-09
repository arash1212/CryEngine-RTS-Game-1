#include "StdAfx.h"
#include "MoveAction.h"
#include "GamePlugin.h"

#include <Components/Selectables/Units/BaseUnit.h>

MoveAction::MoveAction(IEntity* entity, Vec3 movePosition, bool run)
{
	this->m_pEntity = entity;
	this->m_movePosition = movePosition;
	this->bRun = run;
}

void MoveAction::Execute()
{
	BaseUnitComponent* unit = m_pEntity->GetComponent<BaseUnitComponent>();
	if (unit) {
		unit->MoveTo(m_movePosition, bRun);
	}
}

void MoveAction::Cancel()
{
	BaseUnitComponent* unit = m_pEntity->GetComponent<BaseUnitComponent>();
	if (unit) {
		unit->StopMoving();
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
