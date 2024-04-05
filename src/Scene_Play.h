#pragma once

#include "Scene_Old.h"
#include "EntityManager.h"

#include <map>
#include <memory>
#include <string>
#include "imgui.h"
#include "imgui-SFML.h"

class Scene_Play : public Scene_Old
{

protected:

	std::shared_ptr<Entity>	player_;
	std::string	level_path_;
	Vec2 mouse_pos_;

public:
	Scene_Play();
	Scene_Play(const std::string& filename);
	void LoadScene(const std::string& filename);
	void update();
	void onEnd();
	void sUserInput();
	void sPhysics();
	void sCollision();
	void sAnimation();
	void sRender();
	void sRenderColliders();
	void sMovement();
	void spawnPlayer();
};