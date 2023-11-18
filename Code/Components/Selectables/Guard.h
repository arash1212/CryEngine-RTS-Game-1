#pragma once

class AIControllerComponent;
class ResourceManagerComponent;
class ResourceCollectorComponent;
class ResourceManagerComponent;
class OwnerInfoComponent;

class GuardComponent final : public IEntityComponent
{

public:
	GuardComponent() = default;
	virtual ~GuardComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<GuardComponent>& desc)
	{
		desc.SetGUID("{27597E74-A429-4B34-B949-3F9A745A660F}"_cry_guid);
		desc.SetEditorCategory("Units");
	}


private:
	AIControllerComponent* m_pAIController = nullptr;
	OwnerInfoComponent* m_pOwnerInfoComponent = nullptr;

private:
	IEntity* m_pAssignedGuardPost = nullptr;
	bool bIsEnteredGuardPost = false;
	bool bIsReachedAssignedPoint = false;

private:
	void EnterAssignedGuardPost();

public:
	void AssignGuardPost(IEntity* guardPost);
	IEntity* GetAssignedGuardPost();
	void CancelAssignedGuardPost();

	bool HasEnteredGuardPost();
	void SetHasEnteredGuardPost(bool hasEntered);
};

