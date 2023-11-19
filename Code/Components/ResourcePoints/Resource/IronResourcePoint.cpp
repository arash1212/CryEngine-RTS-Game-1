#include "StdAfx.h"
#include "IronResourcePoint.h"
#include "GamePlugin.h"

#include <UIItems/IBaseUIItem.h>
#include <UIItems/Items/UICancelItem.h>
#include <UIItems/Items/UIChangeStanceItem.h>
#include <UIItems/Items/Buildings/UIHQ1BuildItem.h>
#include <Resources/IResource.h>
#include <Components/ResourcePoints/BaseResourcePoint.h>
#include <Components/Selectables/Selectable.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterIronResourcePointComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(IronResourcePointComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterIronResourcePointComponent);
}

void IronResourcePointComponent::Initialize()
{
	//AnimationComponent Initializations
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), Quat::CreateRotationXYZ(Ang3(DEG2RAD(0), 0, DEG2RAD(0))), Vec3(0)));
	m_pAnimationComponent->SetCharacterFile("Objects/resource/iron/resource_iron.cdf");
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/resourceiron.adb");
	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("ThirdPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Idle");
	m_pAnimationComponent->SetAnimationDrivenMotion(false);
	m_pAnimationComponent->LoadFromDisk();
	m_pAnimationComponent->ResetCharacter();

	//ResourceComponent Initialization
	SetType(EResourceType::IRON);
	SetIsSingleUse(false);
	SetHasCollectingLocation(false);

	//BoxComponent Initialization
	m_pBboxComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CBoxPrimitiveComponent>();
	m_pBboxComponent->m_size = Vec3(1.6f, 1.6f, 1.4f);
	m_pBboxComponent->m_bReactToCollisions = true;

	//Update bounding box
	AABB aabb;
	m_pEntity->GetLocalBounds(aabb);
	Vec3 min = Vec3(aabb.min.x - 1.8f, aabb.min.y - 1.8f, aabb.min.z);
	Vec3 max = Vec3(aabb.max.x + 1.8f, aabb.max.y + 1.6f, aabb.max.z);
	AABB newAABB = AABB(min, max);
	m_pEntity->SetLocalBounds(newAABB, true);

	//AnimationComponent Initialization
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();

	//SelectableComponent Initialization
	m_pSelectableComponent = m_pEntity->GetOrCreateComponent<SelectableComponent>();
	m_pSelectableComponent->SetIsBuilding(true);

	//CollectingLocationAttachment Initialization
	if (bHasCollectingLocation) {
		m_pCollectingLocationAttachment = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("collectingLocation");
	}

	//Physicalize
	SEntityPhysicalizeParams physParams;
	physParams.type = PE_STATIC;
	physParams.mass = 38000.f;
	m_pEntity->Physicalize(physParams);
}

Cry::Entity::EventFlags IronResourcePointComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void IronResourcePointComponent::ProcessEvent(const SEntityEvent& event)
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