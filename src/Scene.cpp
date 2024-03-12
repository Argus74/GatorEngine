#include "Scene.h"

// Constructors
Scene::Scene() : m_game(nullptr) {}

Scene::Scene(GameEngine* gameEngine) : m_game(gameEngine) {}

// Set the paused state of the scene
void Scene::setPaused(bool paused) {
	m_paused = paused;
}

// Execute an action within the scene
void Scene::doAction(const Action& action) {
	// Delegate the action handling to the sDoAction method, which should be implemented in derived classes
	sDoAction(action);
}

// Simulate a number of frames within the scene, typically used for fast-forwarding or AI simulation
void Scene::simulate(const size_t frames) {
	for (size_t i = 0; i < frames; ++i) {
		update();  // Call the update method, which should be implemented in derived classes
	}
}

// Register a keyboard input to a specific action name
void Scene::registerAction(int inputKey, const std::string& actionName) {
	m_actionMap[inputKey] = actionName;
}

// Retrieve the width of the scene - this may need to be linked to your window or viewport size
size_t Scene::width() const {
	// LR: Temporary logic, check width
	return 800;  // Example width
}

// Retrieve the height of the scene - this may need to be linked to your window or viewport size
size_t Scene::height() const {
	// LR: Temporary logic, check height
	return 600;  // Example height
}

// Retrieve the current frame number of the scene
size_t Scene::currentFrame() const {
	return m_currentFrame;
}

// Check if the scene has ended
bool Scene::hasEnded() const {
	return m_hasEnded;
}

// Retrieve the action map of the scene
const ActionMap& Scene::getActionMap() const {
	return m_actionMap;
}

// Method to draw a line within the scene - you will need to implement this based on your graphics library
void Scene::drawLine(const Vec2& p1, const Vec2& p2) {
	void Scene::drawLine(const Vec2 & p1, const Vec2 & p2) {
		sf::VertexArray line(sf::Lines, 2);
		line[0].position = sf::Vector2f(p1.x, p1.y);
		line[1].position = sf::Vector2f(p2.x, p2.y);

		// Set the color of the line
		line[0].color = sf::Color::White;  // Starting point color
		line[1].color = sf::Color::White;  // Ending point color

		// Draw the line on the window
		m_game->window().draw(line);
	}
}
