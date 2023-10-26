#pragma once

static constexpr f32 DEFAULT_MAX_HEALTH = 100.f;

class HealthComponent final : public IEntityComponent
{

public:
	HealthComponent() = default;
	virtual ~HealthComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<HealthComponent>& desc)
	{
		desc.SetGUID("{4B5C760A-FFE7-4F53-A6A0-AB2A4B40E0DD}"_cry_guid);
		desc.SetEditorCategory("Selectables");
	}

private:
	f32 m_maxHealth = DEFAULT_MAX_HEALTH;
	f32 m_currentHealth = m_maxHealth;

public:
	void ApplyDamage(f32 damage);
	void Die();

	void SetMaxHealth(f32 maxHealth);
	f32 GetCurrentHealth();
};
