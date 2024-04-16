#pragma once

#include "Scene.h"
#include "EntityManager.h"

#include <map>
#include <memory>
#include <string>
#include "imgui.h"
#include "imgui-SFML.h"

class Scene_Play : public Scene
{

protected:

	std::shared_ptr<Entity>	m_player;
	std::string	m_levelPath;
	Vec2 m_mousePos;

public:
	Scene_Play();
	Scene_Play(const std::string& filename);
	void LoadScene(const std::string& filename);
	void update();
	void sTouchTrigger();
	void onEnd();
	void sUserInput();
	void sPhysics();
	void sCollision();
	void sAnimation();
	void sRender();
	void sRenderColliders();
	void sMovement();
	void sBackground();
	void spawnPlayer();
};