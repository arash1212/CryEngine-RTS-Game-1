#include "StdAfx.h"
#include "FogOfWar.h"
#include "GamePlugin.h"

#include <CryEntitySystem/IEntitySystem.h>
#include <Components/Info/OwnerInfo.h>
#include <Components/Selectables/Health.h>
#include <Components/Selectables/Attacker.h>
#include <Utils/EntityUtils.h>
#include <CryRenderer/ITexture.h>
#include <Cry3DEngine/IIndexedMesh.h>
#include <Components/Managers/ResourceManager.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterFogOfWarComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(FogOfWarComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterFogOfWarComponent);
}

void FogOfWarComponent::Initialize()
{
	//ResourceManagerComponent Initialization
	m_pResourceManagerComponent = m_pEntity->GetComponent<ResourceManagerComponent>();

	IStatObj* pStatObj = gEnv->p3DEngine->CreateStatObj();
	pStatObj->SetFilePath("Objects/effects/fogofwar/fogofwar.cgf");
	m_pEntity->SetStatObj(pStatObj, 0, false);
}

Cry::Entity::EventFlags FogOfWarComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::PhysicsCollision |
		Cry::Entity::EEvent::Reset;
}

void FogOfWarComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];

		//Test();
	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	case Cry::Entity::EEvent::PhysicsCollision:
	{

	}break;
	default:
		break;
	}
}

void FogOfWarComponent::Test()
{
	if (!m_pResourceManagerComponent) {
		return;
	}
	//Objects/effects/fogofwar/fogofwar.cgf
	IEntity* entity = gEnv->pEntitySystem->FindEntityByName("FogOfWar");
	if (!entity) {
		return;
	}
	IStatObj* pStatObj = entity->GetStatObj(0);
	if (!pStatObj) {
		return;
	}
	IRenderMesh* renderMesh = pStatObj->GetRenderMesh();
	if (!renderMesh) {
		return;
	}
	IIndexedMesh* indexedMesh = renderMesh->GetIndexedMesh();
	CMesh* mesh = indexedMesh->GetMesh();
	CryLog("count vertex : %i", mesh->GetVertexCount());

	Vec3* const vertices = mesh->GetStreamPtr<Vec3>(CMesh::POSITIONS);
	SMeshColor* const colors = mesh->GetStreamPtr<SMeshColor>(CMesh::COLORS_0);

	int max = num + 250 <= mesh->GetVertexCount() ? num + 250 : mesh->GetVertexCount();
	for (int i = num; i < max; i++) {
		for (IEntity* ownedEntity : m_pResourceManagerComponent->GetOwnedEntities()) {
			float distance = ownedEntity->GetWorldPos().GetDistance(entity->GetWorldPos() + vertices[i]);
			if (distance <= 30) {
				SMeshColor color;
				color.MaskA(0);
				colors[i] = color;
				CryLog("true");
			}
			else if (distance > 30) {
				//vertices[i] = Vec3(vertices[i].x, vertices[i].y, defaultZ);
			}
		}
	}

	num = max;
	if (num >= mesh->GetVertexCount()) {
		num = 0;
	}
	IStatObj* pStatObj2 = gEnv->p3DEngine->CreateStatObj();
	IIndexedMesh* mIndexedMesh = pStatObj2->GetIndexedMesh();
	mIndexedMesh->SetMesh(*mesh);
	pStatObj2->Invalidate();
	entity->SetStatObj(pStatObj2, 0, false);
}
