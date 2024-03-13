#pragma once

#include "Scene.h"
#include <map>
#include <memory>
#include <dequeu>

#include "EntityManager.h"

class Scene)Menu : public Scene
{
protected:

	std::string					m_title;
	std::vector<std::string>	m_menuString;
	std::vector<std::string>	m_levelPaths;
	sf::Text					m_menuText;
	size_t						m_selectMenuIndex = 0;

	void init();
	void update();
	void onEnd();
	void sDoAction(const Action& action);

public:
	Scene_Menu(GameEngine* gameEngine = nullptr);
	void sRender;
}