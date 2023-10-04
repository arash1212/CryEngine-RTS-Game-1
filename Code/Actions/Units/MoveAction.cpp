#include "StdAfx.h"
#include "MoveAction.h"
#include "GamePlugin.h"

#include <Components/Selectables/Units/BaseUnit.h>

MoveAction::MoveAction(IEntity* entity, Vec3 movePosition)
{
	this->m_pEntity = entity;
	this->m_movePosition = movePosition;
}

void MoveAction::Execute()
{
	BaseUnitComponent* unit = m_pEntity->GetComponent<BaseUnitComponent>();
	if (unit) {
		unit->MoveTo(m_movePosition);
	}
}

void MoveAction::Cancel()
{
	BaseUnitComponent* unit = m_pEntity->GetComponent<BaseUnitComponent>();
	if (unit) {
		unit->StopMoving();
	}
}

bool MoveAction::IsDone()
{
	return m_pEntity->GetWorldPos().GetDistance(m_movePosition) <= 1;
}
