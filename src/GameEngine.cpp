#include "GameEngine.h"

GameEngine::GameEngine() {}
GameEngine &GameEngine::GetInstance()
{
	// TODO: insert return statement here
	static GameEngine instance_;

	if (!instance_.initialized_)

	{
		instance_.init("assets/scenes/scene1.json");
		instance_.initialized_ = true;
	}
	return instance_;
}

void GameEngine::ChangeScene(const std::string &sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
	if (endCurrentScene)
	{
		currentScene->onEnd();
	}

	m_currentScene = sceneName;
	currentScene = scene;
}

void GameEngine::quit()
{
	m_running = false;
}

void GameEngine::run()
{
	m_running = true;
}

sf::RenderWindow &GameEngine::window()
{
	return m_window;
}

AssetManager &GameEngine::assets()
{
	return m_assets;
}

void GameEngine::init(const std::string &path)
{
	// Just temporary sprites for testing once, we get a file system going we will do this somewhere else
	auto &assetManager = AssetManager::GetInstance();
	assetManager.AddTexture("Ground", "assets/Terrain/Terrain (16x16).png");
	assetManager.AddTexture("Tree", "assets/Terrain/Terrain (16x16).png");
	assetManager.AddTexture("DefaultSprite", "assets/DefaultSprite.png");
	assetManager.AddTexture("DefaultAnimationTexture", "assets/DefaultAnimation.png");
	assetManager.AddTexture("RunningAnimationTexture", "assets/RunningAnimation.png");
	Animation ani = Animation("DefaultAnimation", assetManager.GetTexture("DefaultAnimationTexture"), 11, 1);
	assetManager.AddAnimation("DefaultAnimation", ani);
	Animation ani2 = Animation("RunningAnimation", assetManager.GetTexture("RunningAnimationTexture"), 11, 1);
	assetManager.AddAnimation("RunningAnimation", ani2);
	
	//123
	m_window.setFramerateLimit(60);
}

void GameEngine::update()
{
	sUserInput();
	currentScene->update();
}

void GameEngine::sUserInput()
{
	//sf::Event event;
	//while (m_window.pollEvent(event))
	//{
	//	ImGui::SFML::ProcessEvent(event);

	//	if (event.type == sf::Event::Closed) {
	//		//m_window.close();
	//	}

	//	// Update the SFML's render window dimensions to prevent weird sprite scaling
	//	if (event.type == sf::Event::Resized) {
	//		sf::FloatRect view(0, 0, event.size.width, event.size.height);
	//		m_window.setView(sf::View(view));
	//	}

	//	// The game engine handles the following key presses:
	//	// - Enter: Toggles the game's running state
	//	// - Escape: Closes the game
	//	if (event.type == sf::Event::KeyPressed) {
	//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) 
	//		{
	//			if (m_running)
	//			{
	//				quit();
	//			}
	//			else
	//			{
	//				run();
	//			}
	//		}
	//		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	//		{
	//			m_window.close();
	//		}
	//	}
	//}
}

bool GameEngine::isRunning()
{
	return m_running;
}

bool GameEngine::isKeyPressed(sf::Keyboard::Key key)
{
	return sf::Keyboard::isKeyPressed(key);
}
