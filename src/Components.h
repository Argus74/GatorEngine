#pragma once

#include "AssetManager.h"

class Component
{
public:
	bool has = false;
};

/*class CName : public Component {
public:
	std::string name;
	CName() {};
	CName(const std::string name);
};

class CTransform : public Component {
public:
	Vec2 position, scale, velocity;
	float angle;
	CTransform() : position(Vec2(0, 0)), scale(Vec2(1, 1)), angle(0) {}
	CTransform(const Vec2& pos, const Vec2& scl, float ang)

		: position(pos), scale(scl), angle(ang) {}
};

class CShape : public Component {
public:
	std::string type;
	sf::Color color;
	CShape() : type("Rectangle"), color(sf::Color::White) {}
	CShape(const std::string& t, const sf::Color& c) : type(t), color(c) {}
}; */

class CAnimation : public Component {
	sf::Sprite currentFrame; 

public:

};

class CSprite : public Component {

};