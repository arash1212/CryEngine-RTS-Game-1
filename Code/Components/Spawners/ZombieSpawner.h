#pragma once

class Zombie1UnitComponent;
class OwnerInfoComponent;
class ResourceManagerComponent;

class ZombieSpawnerComponent final : public IEntityComponent
{

public:
	ZombieSpawnerComponent() = default;
	virtual ~ZombieSpawnerComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<ZombieSpawnerComponent>& desc)
	{
		desc.SetGUID("{C788C42A-BBDC-4F36-8C64-67C364AC2E5C}"_cry_guid);
		desc.SetEditorCategory("UI");
	}

private:
	OwnerInfoComponent* m_pOwnerInfoComponent = nullptr;
	ResourceManagerComponent* m_pResourceManagerComponent = nullptr;

private:
	bool bIsGameStarted = false;

	f32 m_timeBetweenSpawningZombies = 5.f;
	f32 m_spawnTimePassed = -30.f;
	int32 m_maxZombiesCount = 30;

public:
	void SpawnZombies();
};