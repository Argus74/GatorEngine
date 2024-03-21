#pragma once
#include "imgui-SFML.h"
#include "SFML/Graphics/Transform.hpp"
#include "Vec2.h"
#include "Component.h"

class CTransform : public Component {
public:
	DECLARE_COMPONENT_NAME("Transform");
	Vec2 position, scale, velocity;
	float angle;
	CTransform() : position(Vec2(0, 0)), scale(Vec2(1, 1)), angle(0) {}
	CTransform(Vec2 pos) : position(pos), scale(Vec2(1, 1)), angle(0) {}
	CTransform(const Vec2& pos, const Vec2& scl, float ang)
		: position(pos), scale(scl), angle(ang) {}
};