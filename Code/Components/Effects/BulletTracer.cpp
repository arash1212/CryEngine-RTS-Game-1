#include "StdAfx.h"
#include "BulletTracer.h"
#include "GamePlugin.h"

#include <CryEntitySystem/IEntitySystem.h>
#include <Components/Info/OwnerInfo.h>
#include <Components/Selectables/Health.h>
#include <Components/Selectables/Attacker.h>
#include <Utils/EntityUtils.h>

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
	m_pMeshComponent->SetTransformMatrix(Matrix34::Create(Vec3(0.2f, 1.0f, 1), Quat::CreateRotationZ(RAD2DEG(90.5f)), Vec3(0)));
	m_pMeshComponent->SetFilePath("Objects/effects/bulletTracer/bullet_tracer_1.cgf");
	m_pMeshComponent->LoadFromDisk();
	m_pMeshComponent->ResetObject();
	
	//m_pEntity->LoadGeometry(0, "Objects/effects/bulletTracer/bullet_tracer_1.cgf");
	//m_pEntity->LoadGeometry(0, "%ENGINE%/EngineAssets/Objects/primitive_sphere.cgf");
	//m_pEntity->SetScale(Vec3(0.5f));

	//OwnerComponent Initialization
	//m_pOwnerInfoComponent = m_pEntity->GetComponent<OwnerInfoComponent>();

	//m_pPointLightComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CPointLightComponent>();
	//m_pPointLightComponent->SetTransformMatrix(Matrix34::Create(Vec3(2), IDENTITY, Vec3(0, 0, 0)));
	//m_pPointLightComponent->GetColorParameters().m_diffuseMultiplier = 0.4f;
	//m_pPointLightComponent->GetColorParameters().m_color = ColorF(1.f, 1.f, 1.f);
	//m_pPointLightComponent->GetOptions().m_attenuationBulbSize = 3.f;
	//m_pPointLightComponent->Enable(true);

	SEntityPhysicalizeParams physParams;
	physParams.type = PE_RIGID;
	physParams.mass = 10000.f;
	m_pEntity->Physicalize(physParams);

	m_pEntity->SetName("BulletTracer");
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
		const float initialVelocity = 400.f;
		impulseAction.impulse = GetEntity()->GetWorldRotation().GetColumn1() * initialVelocity;

		pPhysics->Action(&impulseAction);
	}
}

void BulletTracerComponent::Destroy()
{
	gEnv->pEntitySystem->RemoveEntity(m_pEntity->GetId());
}

void BulletTracerComponent::CheckCollision(const EventPhysCollision* physCollision)
{
	//TODO : hitEntity khod bullet hast ?!
	if (!physCollision) {
		return;
	}
	Destroy();

	IPhysicalEntity* pEntity = *physCollision->pEntity;
	IEntity* hitEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pEntity);
	if (!m_pOwner || !hitEntity) {
		return;
	}

	OwnerInfoComponent* pthisOwnerInfoComponent = m_pOwner->GetComponent<OwnerInfoComponent>();
	AttackerComponent* pAttackerInfoComponent = m_pOwner->GetComponent<AttackerComponent>();
	if (!pAttackerInfoComponent) {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "BulletTracerComponent : (CheckCollision) pAttackerInfoComponent is null !");
		return;
	}



	OwnerInfoComponent* phitOwnerInfoComponent = hitEntity->GetComponent<OwnerInfoComponent>();
	//if (!m_pTarget || m_pTarget->IsGarbage()) {
	//	return;
	//}
	HealthComponent* pHitHealthComponent = hitEntity->GetComponent<HealthComponent>();
	if (!pHitHealthComponent) {
		return;
	}
	if (hitEntity != m_pOwner || phitOwnerInfoComponent && pthisOwnerInfoComponent->GetTeam() != phitOwnerInfoComponent->GetTeam()) {
		//pHitHealthComponent->ApplyDamage(pAttackerInfoComponent->GetDamageAmount());
	}

}

void BulletTracerComponent::SetOwner(IEntity* owner)
{
	this->m_pOwner = owner;
}

void BulletTracerComponent::SetTarget(IEntity* target)
{
	this->m_pTarget = target;
}
