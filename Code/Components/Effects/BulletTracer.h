#pragma once

#include <DefaultComponents/Geometry/StaticMeshComponent.h>

class BulletTracerComponent final : public IEntityComponent
{

public:
	BulletTracerComponent() = default;
	virtual ~BulletTracerComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<BulletTracerComponent>& desc)
	{
		desc.SetGUID("{C77E11BF-E791-4050-A309-3BA25A9E85E8}"_cry_guid);
		desc.SetEditorCategory("Effects");
	}
private:
	Cry::DefaultComponents::CStaticMeshComponent* m_pMeshComponent = nullptr;

	IEntity* m_pOwner = nullptr;
private:
	f32 m_timeBetweenDestroy = 5.0f;
	f32 m_destroyTimePassed = 0.f;
private :
	void Move();
	void Destroy();

public:
	void SetOwner(IEntity* owner);
};