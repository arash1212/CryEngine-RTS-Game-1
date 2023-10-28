#pragma once

#include <DefaultComponents/Cameras/CameraComponent.h>

struct IUIElement;
struct IUIElementEventListener;

class UISelectionBoxComponent final : public IEntityComponent
{

public:
	UISelectionBoxComponent() = default;
	virtual ~UISelectionBoxComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<UISelectionBoxComponent>& desc)
	{
		desc.SetGUID("{6944C2C8-8517-4DFF-9B91-8EEE288ED474}"_cry_guid);
		desc.SetEditorCategory("UI");
	}
private:
	Cry::DefaultComponents::CCameraComponent* m_pCameraComponent = nullptr;
	IUIElement* m_pSelectionBoxUIElement = nullptr;;

private:
	Vec2 m_boxInitPoint = ZERO;

public:
	DynArray<IEntity*> GetEntitiesInsideBox(Vec2 lastPoint);
	void SetBoxInitPosition(Vec2 initPoint);
	void SetCameraComponent(Cry::DefaultComponents::CCameraComponent* cameraComponent);

	Vec2 GetInitPosition();
	bool IsBoxSelectionTriggered(Vec2 currentPoint);
	void SetEventListener(IUIElementEventListener* eventListener);
};