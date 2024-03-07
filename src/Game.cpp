#include "Game.h"
#include "ActionBus.h"

void Game::init()
{   
    m_window.setFramerateLimit(60);
    sEnemySpawner();
}

Game::Game()
{
    init();
}

void Game::update()
{
    EntityManager::GetInstance().update();
	// GLUING NOTE: Call cUserInput() before rest of systems. Also, only call it if the game is playing (not paused).
	sUserInput();
}

void Game::sUserInput()
{
	// Clear the bus from previous frame's input
	ActionBus::GetInstance().Clear();

	sf::Event event;
	while (m_window.pollEvent(event)) 
	{
		ImGui::SFML::ProcessEvent(event);

		if (event.type == sf::Event::Closed) {
			m_window.close();
		}

		// Update the SFML's render window dimensions to prevent weird sprite scaling
		if (event.type == sf::Event::Resized) {
			sf::FloatRect view(0, 0, event.size.width, event.size.height);
			m_window.setView(sf::View(view));
		}

		// Lambda to process key or mouse events
		auto processInputEvent = [](const sf::Event& event, const sf::Event::EventType& eventType) {
			auto& entities = EntityManager::GetInstance().getEntities();
			for (auto& entity : entities) {

				// Skip entities without a cUserInput component
				if (!entity->cUserInput) continue;

				auto findAndDispatch = [entity](auto& inputMap, const auto& eventButton) {
					for (auto& actionKeys : inputMap) {
						if (actionKeys.first == eventButton) {
							ActionBus::GetInstance().Dispatch(entity, actionKeys.second);
						}
					}
				};

				// Find potential actions and dispatch them based on the event type
				if (eventType == sf::Event::KeyPressed) {
					auto& inputMap = entity->cUserInput->keyMap;
					findAndDispatch(inputMap, event.key.code);
				} else {
					auto& inputMap = entity->cUserInput->mouseMap;
					findAndDispatch(inputMap, event.mouseButton.button);
				}
			}
		};

		// Process key or mouse input events with the lambda
		if (event.type == sf::Event::KeyPressed) {
			processInputEvent(event, sf::Event::KeyPressed);
		} else if (event.type == sf::Event::MouseButtonPressed) {
			processInputEvent(event, sf::Event::MouseButtonPressed);
}
	}
}

void Game::sEnemySpawner()
{
    auto& entityManager = EntityManager::GetInstance();
    for (int i = 0; i < 4; i++)
    {
        const auto& entity = entityManager.addEntity("Entity" + std::to_string(i));

        entity->cTransform = std::make_shared<CTransform>(Vec2(110 * i, 120 * i), Vec2(8 * i, 7 * i), 2 * i);
        entity->cName = std::make_shared<CName>("MyEntity " + std::to_string(i));
        entity->cShape = std::make_shared<CShape>("Rectangle", sf::Color(i * 11, i * 11 / 3, i * 11 / 4, 255));
    }
}
