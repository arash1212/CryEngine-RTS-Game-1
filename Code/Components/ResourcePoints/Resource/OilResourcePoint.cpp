#include "StdAfx.h"
#include "OilResourcePoint.h"
#include "GamePlugin.h"

#include <UIItems/IBaseUIItem.h>
#include <UIItems/Items/UICancelItem.h>
#include <UIItems/Items/UIChangeStanceItem.h>
#include <UIItems/Items/Buildings/UIHQ1BuildItem.h>
#include <Resources/IResource.h>
#include <Components/ResourcePoints/ResourcePoint.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterOilResourcePointComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(OilResourcePointComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterOilResourcePointComponent);
}

void OilResourcePointComponent::Initialize()
{
	//AnimationComponent Initializations
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), Quat::CreateRotationXYZ(Ang3(DEG2RAD(0), 0, DEG2RAD(0))), Vec3(0)));
	m_pAnimationComponent->SetCharacterFile("objects/resource/oil/resource_oil.cdf");
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/resourceOil.adb");
	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Idle");
	m_pAnimationComponent->SetAnimationDrivenMotion(false);
	m_pAnimationComponent->LoadFromDisk();
	m_pAnimationComponent->ResetCharacter();

	//ResourceComponent Initialization
	m_pResourcePointComponent = m_pEntity->GetOrCreateComponent<ResourcePointComponent>();
	m_pResourcePointComponent->SetType(EResourceType::OIL);
	m_pResourcePointComponent->SetIsSingleUse(true);
	m_pResourcePointComponent->SetHasCollectingLocation(true);

	//BoxComponent Initialization
	m_pBboxComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CBoxPrimitiveComponent>();
	m_pBboxComponent->m_size = Vec3(2.1f, 3.4f, 1.0f);
	m_pBboxComponent->m_bReactToCollisions = true;

	//Update bounding box
	AABB aabb;
	m_pEntity->GetLocalBounds(aabb);
	Vec3 min = Vec3(aabb.min.x - 4, aabb.min.y - 4, aabb.min.z);
	Vec3 max = Vec3(aabb.max.x + 4.5f, aabb.max.y + 3, aabb.max.z);
	AABB newAABB = AABB(min, max);
	m_pEntity->SetLocalBounds(newAABB, true);

	//Physicalize
	SEntityPhysicalizeParams physParams;
	physParams.type = PE_STATIC;
	physParams.mass = 38000.f;
	m_pEntity->Physicalize(physParams);
}

Cry::Entity::EventFlags OilResourcePointComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void OilResourcePointComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];

	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

