#pragma once

class PlayerComponent;

enum class EPlayer {
	PLAYER1,
	PLAYER2,
	PLAYER3,
	PLAYER4,
	PLAYER5,
	PLAYER6,
	FERAL
};

enum class EPlayerTeam {
	TEAM1,
	TEAM2,
	TEAM3,
	TEAM4,
	TEAM5,
	TEAM6,
	FERAL
};

enum class EPlayerFaction {
	FACTION1,
	FERAL
};

struct SOwnerInfo {
public:
	EPlayer m_pPlayer = EPlayer::PLAYER1;
	EPlayerTeam m_pTeam = EPlayerTeam::TEAM1;
	EPlayerFaction m_pFaction = EPlayerFaction::FACTION1;
};

class OwnerInfoComponent final : public IEntityComponent
{

public:
	OwnerInfoComponent() = default;
	virtual ~OwnerInfoComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<OwnerInfoComponent>& desc)
	{
		desc.SetGUID("{6E54F73F-DCA2-415C-9B37-3DE35358DF77}"_cry_guid);
		desc.SetEditorCategory("Info");
	}

private:
	IEntity* m_pOwner = nullptr;
private:
	SOwnerInfo m_pOwnerInfo;
	bool bIsPlayer = false;

public:
	SOwnerInfo GetInfo();
	void SetTeam(EPlayerTeam team);
	void SetFaction(EPlayerFaction faction);

	bool IsEntityHostile(IEntity* entity);
	bool IsPlayer();
	void SetIsPlayer(bool isPlayer);

	void SetOwner(IEntity* owner);
	IEntity* GetOwner();
};