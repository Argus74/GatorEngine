#pragma once

#include "Component.h"
#include "../Vec2.h"
class CCharacter : public Component {
public:
	DECLARE_COMPONENT_NAME("Character");
	float speed;
	Vec2 jump_force;
	bool is_grounded = true;
	CCharacter() : speed(8.0f), jump_force(Vec2(0,10)) {}
	CCharacter(float input_speed, Vec2 input_jump_force) : speed(input_speed), jump_force(input_jump_force) {}
};