#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>

class BaseWeaponComponent final : public IEntityComponent
{

public:
	BaseWeaponComponent() = default;
	virtual ~BaseWeaponComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<BaseWeaponComponent>& desc)
	{
		desc.SetGUID("{3412110F-8212-4ECE-AA25-5CD033751016}"_cry_guid);
		desc.SetEditorCategory("Weapons");
	};

private:
	//Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	IEntityAudioComponent* m_pAudioComponent = nullptr;

protected:
	IAttachment* m_pWeaponAttachment = nullptr;

	//sounds
	int32 m_currentShootSoundNumber = 0;
	Schematyc::CArray<CryAudio::ControlId> m_shootSounds;

protected:
	IEntity* Raycast(Vec3 to);
	void SpawnProjectile(Vec3 pos);

	int32 m_maxShotCount = 10;
	int32 m_shotCount = 0;

	//Timers
	f32 m_timeBetweenShots = 0.04f;
	f32 m_shotTimePassed = 0.f;
	f32 m_timeBetweenShotCountReset = 0.9f;
	f32 m_shotCountResetTimePassed = 0.f;

public:
	void Fire(Vec3 pos);
	void Draw();
	void PutAway();
	
	CryAudio::ControlId GetRandomShootSound();

	string GetAttachmentName();
};