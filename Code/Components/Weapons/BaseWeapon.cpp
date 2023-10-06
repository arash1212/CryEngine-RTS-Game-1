#include "StdAfx.h"
#include "BaseWeapon.h"
#include "GamePlugin.h"

#include <CryPhysics/physinterface.h>
#include <Components/Effects/BulletTracer.h>
#include <Utils/MathUtils.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterBaseWeaponComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(BaseWeaponComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterBaseWeaponComponent);
}

void BaseWeaponComponent::Initialize()
{
	/*
	//AnimationComponent Initializations
	m_pAnimationComponent = m_pEntity->CreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), Quat::CreateRotationXYZ(Ang3(DEG2RAD(90), 0, DEG2RAD(180))), Vec3(0)));
	m_pAnimationComponent->SetCharacterFile("Objects/Weapons/ak47/ak47.cdf");
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/ak47.adb");
	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Idle");
	m_pAnimationComponent->SetAnimationDrivenMotion(false);
	m_pAnimationComponent->LoadFromDisk();
	m_pAnimationComponent->ResetCharacter();
	*/

	//WeaponAttachment Initialization
	m_pWeaponAttachment = m_pEntity->GetComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>()->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName(GetAttachmentName());

	//AudioComponent Initialization
	m_pAudioComponent = m_pEntity->GetOrCreateComponent<IEntityAudioComponent>();

	//Sounds
	m_shootSounds.Insert(0, CryAudio::StringToId("ak-47-shoot-sound"));
	m_shootSounds.Insert(1, CryAudio::StringToId("ak-47-shoot-sound-1"));
	m_shootSounds.Insert(2, CryAudio::StringToId("ak-47-shoot-sound-2"));
	m_shootSounds.Insert(3, CryAudio::StringToId("ak-47-shoot-sound-3"));
	m_shootSounds.Insert(4, CryAudio::StringToId("ak-47-shoot-sound-4"));
	m_shootSounds.Insert(5, CryAudio::StringToId("ak-47-shoot-sound-5"));
	m_shootSounds.Insert(6, CryAudio::StringToId("ak-47-shoot-sound-6"));
	m_shootSounds.Insert(6, CryAudio::StringToId("ak-47-shoot-sound-7"));
}

Cry::Entity::EventFlags BaseWeaponComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void BaseWeaponComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		f32 DeltaTime = event.fParam[0];

		if (m_shotTimePassed < m_timeBetweenShots) {
			m_shotTimePassed += 0.5f * DeltaTime;
		}
		if (m_shotCountResetTimePassed < m_timeBetweenShotCountReset) {
			m_shotCountResetTimePassed += 0.5f * DeltaTime;
		}
		else {
			m_shotCount = 0;
		}

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

IEntity* BaseWeaponComponent::Raycast(Vec3 to)
{
	Vec3 origin = m_pEntity->GetWorldPos();
	Vec3 dir = to - origin;
	int objTypes = ent_all;
	int flags = rwi_colltype_any | rwi_stop_at_pierceable;
	static IPhysicalEntity* pSkippedEntities[10];
	pSkippedEntities[0] = m_pEntity->GetPhysicalEntity();

	ray_hit hit;
	if (gEnv->pPhysicalWorld->RayWorldIntersection(origin, dir * gEnv->p3DEngine->GetMaxViewDistance(), objTypes, flags, &hit, 1, pSkippedEntities, 2)) {
		if (hit.pCollider) {
			IEntity* hitEntity = gEnv->pEntitySystem->GetEntityFromPhysics(hit.pCollider);
			if (hitEntity) {
				CryLog("Weapon Hit :%s", hitEntity->GetName());
				return hitEntity;
			}
		}
	}
	return nullptr;
}

void BaseWeaponComponent::SpawnProjectile(Vec3 pos)
{
	Vec3 origin = m_pWeaponAttachment->GetAttWorldAbsolute().t;
	Vec3 dir = pos - origin;

	//const IDefaultSkeleton& pDefaultSkeleton = m_pWeaponAttachment->GetIAttachmentObject()->GetICharacterInstance()->GetIDefaultSkeleton();
	//int32 muzzleBoneID = pDefaultSkeleton.GetJointIDByName("Muzzle");
	//const ISkeletonPose* pPose = m_pWeaponAttachment->GetIAttachmentObject()->GetICharacterInstance()->GetISkeletonPose();

	SEntitySpawnParams projectileSpawnParams;
	projectileSpawnParams.vPosition = origin;
	projectileSpawnParams.qRotation = Quat::CreateRotationVDir(dir.normalized());
	IEntity* projectileEntity = gEnv->pEntitySystem->SpawnEntity(projectileSpawnParams, true);
	BulletTracerComponent* bullet = projectileEntity->GetOrCreateComponent<BulletTracerComponent>();
	bullet->SetOwner(m_pEntity);
}

void BaseWeaponComponent::Fire(Vec3 pos)
{
	if (m_shotTimePassed >= m_timeBetweenShots && m_shotCount < m_maxShotCount) {
		//Raycast(pos);
		SpawnProjectile(pos);
		m_shotTimePassed = 0;
		m_pAudioComponent->ExecuteTrigger(GetRandomShootSound());

		m_shotCount++;
		m_shotCountResetTimePassed = 0.f;
	}
}


void BaseWeaponComponent::Draw()
{
	if (!m_pWeaponAttachment) {
		return;
	}
	m_pWeaponAttachment->HideAttachment(0);
}

void BaseWeaponComponent::PutAway()
{
	if (!m_pWeaponAttachment) {
		return;
	}
	m_pWeaponAttachment->HideAttachment(1);
}

string BaseWeaponComponent::GetAttachmentName()
{
	return "ak47";
}

CryAudio::ControlId BaseWeaponComponent::GetRandomShootSound()
{
	int32 randomNum = 0;
	if (m_shootSounds.Size() > 1) {
		randomNum = MathUtils::GetRandomInt(0, m_shootSounds.Size() - 1);
	}
	return m_shootSounds.At(randomNum);
}