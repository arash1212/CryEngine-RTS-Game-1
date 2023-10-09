#include "StdAfx.h"
#include "Selectable.h"
#include "GamePlugin.h"

#include <UIItems/IBaseUIItem.h>
#include <UIItems/Items/UICancelItem.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

namespace
{
	static void RegisterSelectableComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(SelectableComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterSelectableComponent);
}

void SelectableComponent::Initialize()
{
	//SelectionDecalComponent Initialization
	m_pSelectionDecalComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CDecalComponent>();
	m_pSelectionDecalComponent->SetTransformMatrix(Matrix34::Create(Vec3(1), IDENTITY, Vec3(0)));
	m_pSelectionDecalComponent->SetMaterialFileName("Materials/decals/selection/selectionbox_decal_material_1.mtl");
	m_pSelectionDecalComponent->Spawn();

	m_pRenderNode = m_pEntity->GetRenderNode();

	//UIItems
	m_pAllUIItems.push_back(new UICancelItem(m_pEntity));
}

Cry::Entity::EventFlags SelectableComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void SelectableComponent::ProcessEvent(const SEntityEvent& event)
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

void SelectableComponent::Select()
{
	bIsSelected = true;
	m_pSelectionDecalComponent->Spawn();

	m_pRenderNode->m_nHUDSilhouettesParam = m_greenColor;
}

void SelectableComponent::DeSelect()
{
	bIsSelected = false;
	m_pSelectionDecalComponent->Remove();

	m_pRenderNode->m_nHUDSilhouettesParam = m_blackColor;
}

bool SelectableComponent::IsSelected()
{
	return bIsSelected;
}

DynArray<IBaseUIItem*> SelectableComponent::GetUIItems()
{
	return m_pAllUIItems;
}
