#pragma once

class OwnerInfoComponent;

struct IUIElement;

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
	OwnerInfoComponent* m_pOwnerInfoComponent = nullptr;

private:
	f32 m_maxHealth = DEFAULT_MAX_HEALTH;
	f32 m_currentHealth = m_maxHealth;
	f32 m_lastHealthUpdateAmount = 0;

	bool bIsConsumesFood = false;
	bool bHealthBarAdded = false;

	int32 m_healthbarIndex = -1;

	IUIElement* m_pHealthbarUIElement = nullptr;

public:
	void ApplyDamage(f32 damage);
	void Die();

	void SetMaxHealth(f32 maxHealth);
	f32 GetCurrentHealth();

	bool IsConsumesFood();
	void SetConsumesFood(bool consumesFood);

	void AddHealthBar(bool isRed);
	void HideHealthBar();
	void ShowHealthBar();
	void SetHealthbarPosition(f32 x, f32 y);
	void SetHealthAmount(int32 health, bool isRed);
	void ClearAllHealthbars();
};
