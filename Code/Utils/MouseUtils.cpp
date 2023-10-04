#include "StdAfx.h"
#include "MouseUtils.h"
#include "GamePlugin.h"

#include <CryInput/IInput.h>
#include <CryInput/IHardwareMouse.h>

#include <CryPhysics/physinterface.h>
#include <CryRenderer/IRenderer.h>
#include <CryGame/IGameFramework.h>
#include <CryEntitySystem/IEntitySystem.h>

Vec2 MouseUtils::GetCursorPosition()
{
	IHardwareMouse* pHardwareMouse = gEnv->pHardwareMouse;

	Vec2 currentPos;
	pHardwareMouse->GetHardwareMouseClientPosition(&currentPos.x, &currentPos.y);

	return currentPos;
}

Vec3 MouseUtils::GetPositionUnderCursor()
{
	IHardwareMouse* pHardwareMouse = gEnv->pHardwareMouse;

	Vec2 currentPos;
	pHardwareMouse->GetHardwareMouseClientPosition(&currentPos.x, &currentPos.y);
	currentPos.y = gEnv->pRenderer->GetHeight() - currentPos.y;

	Vec3 p1;
	gEnv->pRenderer->UnProjectFromScreen(currentPos.x, currentPos.y, 0, &p1.x, &p1.y, &p1.z);

	Vec3 p2;
	gEnv->pRenderer->UnProjectFromScreen(currentPos.x, currentPos.y, 1, &p2.x, &p2.y, &p2.z);

	Vec3 dir = p2 - p1;

	int objTypes = ent_all;
	int flags = rwi_colltype_any | rwi_stop_at_pierceable;
	ray_hit hit;

	IPersistantDebug* pd = gEnv->pGameFramework->GetIPersistantDebug();
	if (gEnv->pPhysicalWorld->RayWorldIntersection(p1, dir, objTypes, flags, &hit, 1)) {
		if (hit.pCollider) {
			pd->Begin("mousePosTest", true);
			pd->AddSphere(hit.pt, 0.4f, ColorF(1, 0, 0), 1);
			return hit.pt;
		}
	}
	return ZERO;
}

IEntity* MouseUtils::GeetActorUnderCursor()
{
	IHardwareMouse* pHardwareMouse = gEnv->pHardwareMouse;

	Vec2 currentPos;
	pHardwareMouse->GetHardwareMouseClientPosition(&currentPos.x, &currentPos.y);
	currentPos.y = gEnv->pRenderer->GetHeight() - currentPos.y;

	Vec3 p1;
	gEnv->pRenderer->UnProjectFromScreen(currentPos.x, currentPos.y, 0, &p1.x, &p1.y, &p1.z);

	Vec3 p2;
	gEnv->pRenderer->UnProjectFromScreen(currentPos.x, currentPos.y, 1, &p2.x, &p2.y, &p2.z);

	Vec3 dir = p2 - p1;

	int objTypes = ent_all;
	int flags = rwi_colltype_any | rwi_stop_at_pierceable;
	ray_hit hit;

	if (gEnv->pPhysicalWorld->RayWorldIntersection(p1, dir, objTypes, flags, &hit, 1)) {
		if (hit.pCollider) {
			IEntity* hitEntity = gEnv->pEntitySystem->GetEntityFromPhysics(hit.pCollider);
			return hitEntity;
		}
	}

	return nullptr;
}
