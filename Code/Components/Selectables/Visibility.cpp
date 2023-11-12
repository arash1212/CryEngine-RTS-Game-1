#include "StdAfx.h"
#include "Visibility.h"
#include "GamePlugin.h"

#include <Components/Managers/ResourceManager.h>
#include <Utils/EntityUtils.h>

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterVisibilityComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(VisibilityComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterVisibilityComponent);
}


void VisibilityComponent::Initialize()
{
	//AnimationComponent Initialization
	m_pAnimationComponent = m_pEntity->GetComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();

	//AttachmentManager Initialization
	m_pAttachmentManager = m_pAnimationComponent->GetCharacter()->GetIAttachmentManager();

	//Default materials Initializations
	int32 attachmentCount = m_pAttachmentManager->GetAttachmentCount();
	for (int32 i = 0; i < attachmentCount; i++) {
		int32 index = i > 0 ? i - 1 : 0;
		IAttachment* pAttachment = m_pAttachmentManager->GetInterfaceByIndex(index);
		IAttachmentObject* pAttachmentObject = pAttachment->GetIAttachmentObject();
		if (!pAttachmentObject) {
			continue;
		}
		m_defaultMaterials.append(pAttachmentObject->GetReplacementMaterial());
	}
}

Cry::Entity::EventFlags VisibilityComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void VisibilityComponent::ProcessEvent(const SEntityEvent& event)
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

void VisibilityComponent::SetVisible(bool isVisible)
{
	if (!m_pEntity->IsInitialized()) {
		return;
	}
	if (bIsVisible == isVisible) {
		return;
	}
	bIsVisible = isVisible;

	int32 attachmentCount = m_pAttachmentManager->GetAttachmentCount();
	//m_pEntity->Invisible(isHide);
	if (!bIsVisible) {
		IMaterial* m_pHiddenMaterial = gEnv->p3DEngine->GetMaterialManager()->LoadMaterial(DEFAULT_VISIBILITY_INVISIBLE_MATERIAL);
		for (int32 i = 0; i < attachmentCount; i++) {
			int32 index = i > 0 ? i - 1 : 0;
			m_pAttachmentManager->GetInterfaceByIndex(index)->GetIAttachmentObject()->SetReplacementMaterial(m_pHiddenMaterial);
		}
	}
	else {
		for (int32 i = 0; i < attachmentCount; i++) {
			int32 index = i > 0 ? i - 1 : 0;
			m_pAttachmentManager->GetInterfaceByIndex(index)->GetIAttachmentObject()->SetReplacementMaterial(m_defaultMaterials[index]);
		}
	}
}

bool VisibilityComponent::IsVisible()
{
	return bIsVisible;
}
