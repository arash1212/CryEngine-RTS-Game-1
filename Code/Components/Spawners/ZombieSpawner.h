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

	struct SSpawnPointDefinition
	{
		static void ReflectType(Schematyc::CTypeDesc<SSpawnPointDefinition>& desc)
		{
			desc.SetLabel("SpawnPoint Definition");
			desc.SetDescription("Describes an enemy that can be activated/spawned by the spawner component.");
			desc.SetGUID("{E9D2AE27-3457-4322-B616-6C21CD6814DF}"_cry_guid);
		}

		inline bool Serialize(Serialization::IArchive& archive);

		inline bool operator==(const SSpawnPointDefinition& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SSpawnPointDefinition& rhs) const { return !(*this == rhs); }

		string spawnPointEntityName;
	};
	typedef Schematyc::CArray<SSpawnPointDefinition> SpawnPoints;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<ZombieSpawnerComponent>& desc)
	{
		desc.SetGUID("{C788C42A-BBDC-4F36-8C64-67C364AC2E5C}"_cry_guid);
		desc.SetEditorCategory("UI");
		desc.AddMember(&ZombieSpawnerComponent::m_pSpawnPoints, 'spp', "spawnpoints", "Spawn Points", "Set Spawn Point Names", SpawnPoints());
	}

private:
	OwnerInfoComponent* m_pOwnerInfoComponent = nullptr;
	ResourceManagerComponent* m_pResourceManagerComponent = nullptr;

	SpawnPoints m_pSpawnPoints;
private:
	bool bIsGameStarted = false;

	f32 m_timeBetweenSpawningZombies = -150.f;
	f32 m_spawnTimePassed = -0.f;
	int32 m_maxZombiesCount = 10;
	int32 m_zombiesSpawned = 0;
public:
	void SpawnZombies();
};