#pragma once

class MouseUtils {
public:
	static Vec2 GetCursorPosition();
	static Vec3 GetPositionUnderCursor();
	static IEntity* GetActorUnderCursor();
	static bool IsMouseInsideViewPort();
};