#pragma once
#include "imgui-SFML.h"
#include "SFML/Graphics/Transform.hpp"
#include "Vec2.h"
#include "Component.h"

class CTransform : public Component {
public:
	Vec2 position, scale, velocity;
	float angle;
	CTransform() : Component("Transform"), position(Vec2(0, 0)), scale(Vec2(1, 1)), angle(0) {}
	CTransform(Vec2 pos) : Component("Transform"), position(pos), scale(Vec2(1, 1)), angle(0) {}
	CTransform(const Vec2& pos, const Vec2& scl, float ang)
		: Component("Transform"), position(pos), scale(scl), angle(ang) {}
};