#include "StdAfx.h"
#include "BulletTracer.h"
#include "GamePlugin.h"

#include <CryEntitySystem/IEntitySystem.h>
#include <Components/Info/OwnerInfo.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterBulletTracerComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(BulletTracerComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterBulletTracerComponent);
}

void BulletTracerComponent::Initialize()
{
	m_pMeshComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CStaticMeshComponent>();
	m_pMeshComponent->SetTransformMatrix(Matrix34::Create(Vec3(0.5f), Quat::CreateRotationZ(RAD2DEG(90.5f)), Vec3(0)));
	m_pMeshComponent->SetFilePath("Objects/effects/bulletTracer/bullet_tracer_1.cgf");
	m_pMeshComponent->LoadFromDisk();
	m_pMeshComponent->ResetObject();
	
	//m_pEntity->LoadGeometry(0, "Objects/effects/bulletTracer/bullet_tracer_1.cgf");
	//m_pEntity->LoadGeometry(0, "%ENGINE%/EngineAssets/Objects/primitive_sphere.cgf");
	//m_pEntity->SetScale(Vec3(0.5f));

	//OwnerComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetComponent<OwnerInfoComponent>();

	SEntityPhysicalizeParams physParams;
	physParams.type = PE_RIGID;
	physParams.mass = 18000.f;
	m_pEntity->Physicalize(physParams);
}

Cry::Entity::EventFlags BulletTracerComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::PhysicsCollision |
		Cry::Entity::EEvent::Reset;
}

void BulletTracerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		f32 DeltaTime = event.fParam[0];

		Move();

		//Destroy Projectile
		if (m_destroyTimePassed < m_timeBetweenDestroy) {
			m_destroyTimePassed += 0.5f * DeltaTime;
		}
		else {
			Destroy();
		}

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	case Cry::Entity::EEvent::PhysicsCollision:
	{
		const EventPhysCollision* pPhysCollision = reinterpret_cast<EventPhysCollision*>(event.nParam[0]);
		this->CheckCollision(pPhysCollision);
		
	}break;
	default:
		break;
	}
}

void BulletTracerComponent::Move()
{
	if (auto* pPhysics = GetEntity()->GetPhysics())
	{
		pe_action_impulse impulseAction;
		const float initialVelocity = 3500.f;
		impulseAction.impulse = GetEntity()->GetWorldRotation().GetColumn1() * initialVelocity;

		pPhysics->Action(&impulseAction);
	}
}

void BulletTracerComponent::Destroy()
{
	gEnv->pEntitySystem->RemoveEntity(GetEntityId());
}

void BulletTracerComponent::CheckCollision(const EventPhysCollision* physCollision)
{
	IEntity* hitEntity = gEnv->pEntitySystem->GetEntityFromPhysics(*physCollision->pEntity);
	if (hitEntity && hitEntity == m_pOwner) {
		return;
	}
	Destroy();
}

void BulletTracerComponent::SetOwner(IEntity* owner)
{
	this->m_pOwner = owner;
}
