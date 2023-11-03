#pragma once

#include <DefaultComponents/Geometry/StaticMeshComponent.h>
#include <DefaultComponents/Lights/PointLightComponent.h>

class OwnerInfoComponent;

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
	Cry::DefaultComponents::CPointLightComponent* m_pPointLightComponent = nullptr;

	IEntity* m_pOwner = nullptr;
	IEntity* m_pTarget = nullptr;
	//OwnerShip
//	OwnerInfoComponent* m_pOwnerInfoComponent = nullptr;

private:
	f32 m_timeBetweenDestroy = 0.3f;
	f32 m_destroyTimePassed = 0.f;

private :
	void Move();
	void Destroy();
	void CheckCollision(const EventPhysCollision* physCollision);

public:
	void SetOwner(IEntity* owner);
	void SetTarget(IEntity* target);
};