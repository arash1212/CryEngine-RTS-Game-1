#include "StdAfx.h"
#include "Health.h"
#include "GamePlugin.h"

#include <Components/Info/OwnerInfo.h>
#include <Components/Selectables/Units/Zombie1Unit.h>
#include <Components/Managers/ResourceManager.h>
#include <Components/Selectables/Workplace.h>
#include <Components/Managers/ActionManager.h>

#include <Components/Player/Player.h>

#include "FlashUI/FlashUI.h"
#include "FlashUI/FlashUIElement.h"

#include <Components/UI/Listener/UIElementEventListener.h>
#include <Components/Selectables/Selectable.h>
#include <Utils/EntityUtils.h>
#include <Components/Info/OwnerInfo.h>

#include <CryEntitySystem/IEntitySystem.h>
#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

#include <DefaultComponents/Cameras/CameraComponent.h>

namespace
{
	static void RegisterHealthComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(HealthComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterHealthComponent);
}

void HealthComponent::Initialize()
{
	//Initialize m_pHealthbarUIElement and set it visible
	m_pHealthbarUIElement = gEnv->pFlashUI->GetUIElement("health-bar");
	if (m_pHealthbarUIElement) {
		m_pHealthbarUIElement->SetVisible(true);
	}
	else {
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "HealthComponent : (Initialize) m_pHealthbarUIElement is null !");
	}

	//OwnerInfoComponent Initialization
	m_pOwnerInfoComponent = m_pEntity->GetComponent<OwnerInfoComponent>();
}

Cry::Entity::EventFlags HealthComponent::GetEventMask() const
{
	return
		Cry::Entity::EEvent::GameplayStarted |
		Cry::Entity::EEvent::Update |
		Cry::Entity::EEvent::Reset;
}

void HealthComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted: {

	}break;
	case Cry::Entity::EEvent::Update: {
		f32 DeltaTime = event.fParam[0];
		if (m_entityRemoveTimePassed < m_timeBetweenRemoveingEntity) {
			m_entityRemoveTimePassed += 0.5f * DeltaTime;
		}

		UpdateProgressAmount();
		Die();

		//TODO: player hamishe yeki hast ?
		IEntity* player = gEnv->pEntitySystem->FindEntityByName(PLAYER_ENTITY_NAME);
		if (!player) {
			return;
		}
		Cry::DefaultComponents::CCameraComponent* camera = player->GetComponent<Cry::DefaultComponents::CCameraComponent>();
		if (!camera) {
			return;
		}
		if (!m_pOwnerInfoComponent) {
			return;
		}
		OwnerInfoComponent* pPlayerOwnerInfo = player->GetComponent<OwnerInfoComponent>();
		if (!pPlayerOwnerInfo) {
			return;
		}
		if (EntityUtils::IsEntityInsideViewPort(camera, m_pEntity) && m_currentHealth > 0) {
			ShowHealthBar();
			m_entityRemoveTimePassed = 0;
		}
		else if (!EntityUtils::IsEntityInsideViewPort(camera, m_pEntity) || m_currentHealth <= 0) {
			HideHealthBar();
			return;
		}

		if ( m_pHealthbarUIElement) {
			bool bIsRed = pPlayerOwnerInfo->GetTeam() != m_pOwnerInfoComponent->GetTeam();
			AddHealthBar(bIsRed);
			//ShowHealthBar();
			Vec3 flashPos;
			Vec2 borders;
			float scale = 0;
			Vec3 pos = m_pEntity->GetWorldPos();
			m_pHealthbarUIElement->WorldToFlash(camera->GetCamera().GetMatrix(), pos, flashPos, borders, scale);

			if (m_lastHealthUpdateAmount != m_currentHealth) {
				SetHealthAmount((int32)m_currentHealth, bIsRed);
			}
			SetHealthbarPosition(flashPos.x , flashPos.y - 15);
		}
		else {
			//HideHealthBar();
		}
	}break;
	case Cry::Entity::EEvent::Reset: {

	}break;
	default:
		break;
	}
}

void HealthComponent::UpdateProgressAmount()
{
	ActionManagerComponent* pActionManagerComponent = m_pEntity->GetComponent<ActionManagerComponent>();
	if (!pActionManagerComponent) {
		return;
	}
	if (!pActionManagerComponent->GetCurrentAction()) {
		if (m_lastProgressbarUpdateAmount != 0) {
			SetProgressAmount(0, 100);
			m_lastProgressbarUpdateAmount = 0;
		}
		return;
	}

	//if (!pActionManagerComponent->GetCurrentAction() || pActionManagerComponent->GetCurrentAction()->GetProgressAmount() == 0) {
	//	HideProgressbar();
	//}

	if (m_lastProgressbarUpdateAmount == pActionManagerComponent->GetCurrentAction()->GetProgressAmount()) {
		return;
	}

	SetProgressAmount(pActionManagerComponent->GetCurrentAction()->GetProgressAmount(), pActionManagerComponent->GetCurrentAction()->GetMaxProgressAmount());
	m_lastProgressbarUpdateAmount = pActionManagerComponent->GetCurrentAction()->GetProgressAmount();
	//ShowProgressbar();
}

void HealthComponent::ApplyDamage(f32 damage)
{
	this->m_currentHealth = CLAMP(m_currentHealth - damage, 0, m_maxHealth);

	//if (m_currentHealth <= 0) {
		
	//}
}

void HealthComponent::Die()
{
	if (m_currentHealth <= 0 && m_entityRemoveTimePassed >= m_timeBetweenRemoveingEntity) {
		HideHealthBar();
		EntityUtils::RemoveEntity(m_pEntity);
	}
}

void HealthComponent::SetMaxHealth(f32 maxHealth)
{
	this->m_maxHealth = maxHealth;
	this->m_currentHealth = maxHealth;
}

f32 HealthComponent::GetCurrentHealth()
{
	return m_currentHealth;
}

bool HealthComponent::IsConsumesFood()
{
	return bIsConsumesFood;
}

void HealthComponent::SetConsumesFood(bool consumesFood)
{
	this->bIsConsumesFood = consumesFood;
}

void HealthComponent::AddHealthBar(bool isRed)
{
	if (bHealthBarAdded) {
		return;
	}

	//Input arguments
	SUIArguments args;
	args.AddArgument(0);
	args.AddArgument(0);
	args.AddArgument(isRed);

	//Returns index
	TUIData data;

	m_pHealthbarUIElement->CallFunction("AddHealthbar", args, &data);

	//Set Index
	int index = -1;
	data.GetValueWithConversion(index);
	m_healthbarIndex = index;

	bHealthBarAdded = true;

	WorkplaceComponent* pWorkplaceComponent = m_pEntity->GetComponent<WorkplaceComponent>();
	if (pWorkplaceComponent) {
		for (int32 i = 0; i < pWorkplaceComponent->GetMaxWorkersCount(); i++) {
			AddWorkerSlot(i, false);
		}
	}
}

void HealthComponent::HideHealthBar()
{
	SUIArguments args;
	args.AddArgument(m_healthbarIndex);

	m_pHealthbarUIElement->CallFunction("HideHealthbar", args);
}

void HealthComponent::ShowHealthBar()
{
	SUIArguments args;
	args.AddArgument(m_healthbarIndex);

	m_pHealthbarUIElement->CallFunction("ShowHealthbar", args);
}

void HealthComponent::SetHealthbarPosition(f32 x, f32 y)
{
	SUIArguments args;
	args.AddArgument(m_healthbarIndex);
	args.AddArgument(x);
	args.AddArgument(y);

	m_pHealthbarUIElement->CallFunction("SetPosition", args);
}

void HealthComponent::SetHealthAmount(int32 health, bool isRed)
{
	SUIArguments args;
	args.AddArgument(m_healthbarIndex);
	args.AddArgument(m_currentHealth / m_maxHealth);
	args.AddArgument(isRed);

	m_lastHealthUpdateAmount = m_currentHealth;
	m_pHealthbarUIElement->CallFunction("SetHealthAmount", args);
}

void HealthComponent::ClearAllHealthbars()
{
	m_pHealthbarUIElement->CallFunction("ClearHealthbars");
}

void HealthComponent::AddWorkerSlot(int32 slotNumber, bool isFilled)
{
	SUIArguments args;
	args.AddArgument(m_healthbarIndex);
	args.AddArgument(slotNumber);
	args.AddArgument(isFilled);

	m_pHealthbarUIElement->CallFunction("AddWorkerSlot", args);
}

void HealthComponent::ClearWorkersSlots()
{
	SUIArguments args;
	args.AddArgument(m_healthbarIndex);

	m_pHealthbarUIElement->CallFunction("ClearWorkerSlots", args);
}

void HealthComponent::SetProgressAmount(f32 progressAmount, f32 maxProgressAmount)
{
	SUIArguments args;
	args.AddArgument(m_healthbarIndex);
	args.AddArgument(progressAmount / maxProgressAmount);

	m_pHealthbarUIElement->CallFunction("SetProgressAmount", args);
}

void HealthComponent::HideProgressbar()
{
	SUIArguments args;
	args.AddArgument(m_healthbarIndex);

	m_pHealthbarUIElement->CallFunction("HideProgressbar", args);
}

void HealthComponent::ShowProgressbar()
{
	SUIArguments args;
	args.AddArgument(m_healthbarIndex);

	m_pHealthbarUIElement->CallFunction("ShowProgressbar", args);
}
