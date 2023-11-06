#include "StdAfx.h"
#include "Selectable.h"
#include "GamePlugin.h"

#include <UIItems/IBaseUIItem.h>
#include <UIItems/Items/UICancelItem.h>
#include <UIItems/Items/UIChangeStanceItem.h>
#include <UIItems/Items/Buildings/UIHQ1BuildItem.h>

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
	m_pSelectionDecalComponent->SetMaterialFileName(SELECTION_UNIT_DECAL_MATERIAL);
	m_pSelectionDecalComponent->SetSortPriority(60);
	m_pSelectionDecalComponent->SetDepth(10);
	//m_pSelectionDecalComponent->Spawn();

	//RenderNode Initialization
	m_pRenderNode = m_pEntity->GetRenderNode();

	//GeneralUIItems
	AddGeneralUIItem(new UIChangeStanceItem(m_pEntity));
	AddGeneralUIItem(new UICancelItem(m_pEntity));
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
		DeSelect();

	}break;
	case Cry::Entity::EEvent::Update: {
		//f32 DeltaTime = event.fParam[0];


	}break;
	case Cry::Entity::EEvent::Reset: {
		DeSelect();

	}break;
	default:
		break;
	}
}

void SelectableComponent::Select()
{
	bIsSelected = true;
	m_pSelectionDecalComponent->Spawn();
	HighLightGreen();
}

void SelectableComponent::DeSelect()
{
	bIsSelected = false;
	m_pSelectionDecalComponent->Remove();
	HighLightBlack();
}

bool SelectableComponent::IsSelected()
{
	return bIsSelected;
}

void SelectableComponent::SetIsBuilding(bool isBuilding)
{
	this->bIsBuilding = isBuilding;
	if (bIsBuilding) {
		m_pGeneralUIItems.clear();
	}
}

bool SelectableComponent::IsBuilding()
{
	return bIsBuilding;
}

void SelectableComponent::AddUIItem(IBaseUIItem* item)
{
	this->m_pUnitSpecificUIItems.append(item);
}

void SelectableComponent::AddGeneralUIItem(IBaseUIItem* item)
{
	this->m_pGeneralUIItems.append(item);
}

DynArray<IBaseUIItem*> SelectableComponent::GetAllUIItems()
{
	return m_pUnitSpecificUIItems + m_pGeneralUIItems;
}

DynArray<IBaseUIItem*> SelectableComponent::GetGeneralUIItems()
{
	return m_pGeneralUIItems;
}

void SelectableComponent::SetDecalSize(Vec3 size)
{
	m_pSelectionDecalComponent->SetTransformMatrix(Matrix34::Create(size, IDENTITY, Vec3(0)));
}

void SelectableComponent::HighLightGreen()
{
	m_pRenderNode->m_nHUDSilhouettesParam = m_greenColor;
}

void SelectableComponent::HighLightBlack()
{
	m_pRenderNode->m_nHUDSilhouettesParam = m_blackColor;
}

void SelectableComponent::MouseOver()
{
	m_pRenderNode->m_nHUDSilhouettesParam = m_greenColor;
	//m_pRenderNode->m_nHUDSilhouettesParam = m_blackColor;
}

void SelectableComponent::RotateSelectionDecal()
{
	if (!m_pSelectionDecalComponent) {
		return;
	}
	if (!bIsSelected) {
		return;
	}
	Quat oldRot = m_pSelectionDecalComponent->GetTransform().get()->GetRotation().ToQuat();
	Quat newRot = Quat::CreateRotationZ(oldRot.GetRotZ() + 0.3f * gEnv->pTimer->GetFrameTime());
	m_pSelectionDecalComponent->SetTransformMatrix(Matrix34::Create(m_pSelectionDecalComponent->GetTransformMatrix().GetScale(), newRot, m_pSelectionDecalComponent->GetTransformMatrix().GetTranslation()));
	m_pSelectionDecalComponent->Spawn();
}