#pragma once
#include "imgui-SFML.h"
#include "SFML/Graphics/Transform.hpp"
#include "Vec2.h"
class CTransform {

public:

	Vec2 position = {0, 0};
	Vec2 velocity = {0, 0};
	CTransform() {};
	CTransform(Vec2 position, Vec2 velocity);
	CTransform(const Vec2 position, const Vec2 velocity) : position(position), velocity(velocity) {};
};