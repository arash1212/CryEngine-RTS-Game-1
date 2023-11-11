#include "StdAfx.h"
#include "WoodResourcePoint.h"
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
	static void RegisterWoodResourcePointComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(WoodResourcePointComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterWoodResourcePointComponent);
}

void WoodResourcePointComponent::Initialize()
{
	//AnimationComponent Initializations
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), Quat::CreateRotationXYZ(Ang3(DEG2RAD(0), 0, DEG2RAD(0))), Vec3(0)));
	m_pAnimationComponent->SetCharacterFile("objects/resource/tree1/tree1.cdf");
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/resourceOil.adb");
	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Idle");
	m_pAnimationComponent->SetAnimationDrivenMotion(false);
	m_pAnimationComponent->LoadFromDisk();
	m_pAnimationComponent->ResetCharacter();

	//ResourceComponent Initialization
	m_pResourceComponent = m_pEntity->GetOrCreateComponent<ResourcePointComponent>();
	m_pResourceComponent->SetType(EResourceType::WOOD);
	m_pResourceComponent->SetIsSingleUse(false);
	m_pResourceComponent->SetHasCollectingLocation(false);

	//BoxComponent Initialization
	m_pBboxComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CBoxPrimitiveComponent>();
	m_pBboxComponent->m_size = Vec3(0.2f, 0.3f, 2.5f);
	m_pBboxComponent->m_bReactToCollisions = true;

	//Update bounding box
	AABB aabb;
	m_pEntity->GetLocalBounds(aabb);
	Vec3 min = Vec3(aabb.min.x - 1.1f, aabb.min.y - 1.1f, aabb.min.z);
	Vec3 max = Vec3(aabb.max.x + 1.5f, aabb.max.y + 1, aabb.max.z);
	AABB newAABB = AABB(min, max);
	m_pEntity->SetLocalBounds(newAABB, true);

	//Physicalize
	SEntityPhysicalizeParams physParams;
	physParams.type = PE_STATIC;
	physParams.mass = 38000.f;
	m_pEntity->Physicalize(physParams);
}

Cry::Entity::EventFlags WoodResourcePointComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void WoodResourcePointComponent::ProcessEvent(const SEntityEvent& event)
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
