#include "Scene_Play.h"
#include "Physics.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"

#include <fstream>
#include <iostream>

bool IsInside(Vec2 pos, std::shared_ptr<Entity> e)
{
	auto s = e->getComponent<CAnimation>().animation.getsize();
	Vec 2 ePos = e->getComponent<CTransform>().pos;

	if (pos.x > ePos.x - s.x / 2 &&
		pos.x < ePos.x - s.x / 2 &&
		pos.y > ePos.y - s.y / 2 &&
		pos.y < ePos.y - s.y / 2)
	{
		std::cout << e->getComponent<CAnimation>().animation.getName() << "\n";
		return true;
	}
	return false;
}
Scene_Play::Scene_Play(GameEngine* gameEngine, const std::string& levelPath)
	: Scene(gameEngine)
	, m_levelPath(levelPath)
{
	init(m_levelPath);
}

void Scene_Play::init(const std::string& levelPath)
{
	registerAction(sf::Keyboard::P, "PAUSE");
	registerAction(sf::Keyboard::Escape, "QUIT");
	registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE"); // Toggle drawing (T)extures
	registerAction(sf::Keyboard::C, "TOGGLE_COLLISION"); // Toggle drawing (C)ollision Boxes
	registerAction(sf::Keyboard::G, "TOGGLE_GRID");	// Toggle drawing (G)rid

	// Example registerAction(sf:Keyboard::W, "JUMP");
	// LR: Need to do left and right
	// TODO: Register all other gameplay Actions (1st implement init function)

	m_gridText.setCharacterSize(12);
	m_gridText.setFont(m_game->assdets().getFont("Tech"));

	loadLevel(levelPath);
}

Vec2 Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
	// TODO: This function takes in a grid (x, y) position and an entity
	//		 Return a Vec2 indicating where the CENTER position of the Entity should be
	//		 You must use the Entity's Animation size to position it correctly
	// 		 The size of the grid width and height is stored in m_gridSize.x and m_gridSize.y
	//		 The bottom-left corner of the Animation should align with the bottom left of the grid cell

	float blx = gridX * m_gridSize.x;
	float bly = height() - gridY * m_gridSize.y);
	Vec2 size = entity->getComponent<CAnimation>().animation.getSize();
	return Vec2(blx + size.x / 2, bly - size.y / 2);
}

void Scene_Play::loadLevel(const std::string& filename)
{
	// reset the entity manager every time we load a level
	m_entityManager = EntityManager();

	// TODO: read in the level file and add the appropriate entities
	//		 use the PlayerConfig struct m_playerConfig to store player properties

	//		 this struct is defined at the top of Scene_Play.h

	// NOTE: all of the code below is sample code which shows you how to
	//		 set up adn use entities with the new syntax, it should be removed

	spawnPlayer();

	// some sample entities
	auto brick = m_entityManager.addEntity("tile");
	// IMPORTANT: always add the CAnimation component first so that gridToMidPixel can computer correctly
	brick->addComponent<CAnimation>(m_game->assets().getAnimation("Brick"), true);
	brick->addComponent<CTransform>(Vec2(96, 480));
	// NOTE: Your final code should position the entity with the grid x,y position read from the file:

	if (brick->getComponent<CAnimation>().animation.getName() == "Brick")
	{
		std::cout << "This could be a good way of identifying if a tile is a brick!\n";
	}

	auto block = m_entityManager.addEntity("tile");
	block->addComponent<CAnimation>(m_game->assets().getAnimation("Block"), true);
	block->addComponent<CTransform>(Vec2(224, 480));
	// add a bounding box, this will now show up if we press the 'C' key
	block->addComponent<CBoundingBox>(m_game->assets().getAnimation("Block").getSize());

	auto question = m_entityManager.addEntity("tile");
	question->addComponent<CAnimation>(m_game->assets().getAnimation("Question"), true);
	question->addComponent<CTranform>(Vec2(352, 480));

	// NOTE: THIS IS INCREDIBLY IMPORTANT PLEASE READ THIS EXAMPLE
	//		 Components are now returned as references rather than pointers
	//		 If you do not specify a reference variable type, it will COPY the component
	//		 Here is an example:
	//
	//		 This will COPY the transform into the variable 'transform1' - it is INCORRECT
	//		 Any changes you make to transform1 will not be changed inside the entity 
	// autotranform1 = entity->get<Transform>()
	//
	// This will REFERENCE the transform with the variable 'transform2' - it is CORRECT
	// Now any changes you make to transform2 will be changed inside the entity
	// auto& transform2 = entity->get<CTransform>()
}

void Scene_Play::spawnPlayer()
{
	// here is a sample player entity which you can use to construct other entities
	m_player = m_entityManager.addEntity("player");
	m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Stand"), true);
	m_player->addComponent<CTransform>(Vec2(224, 352));
	m_player->addComponent<CBoundingBox>(Vec2(48, 48));

	// TODO: be sure to add the remaining components to the player
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity)
{
	// TODO: this should spawn a bullet at the given entity, going in the direction the entity is facing
	auto& transform = entity->getComponent<CTransform>();
	auto bullet = m_entityManager.addEntity("bullet");
	// ... 
}

void Scene_Play::update()
{
	m_entityManager.update();

	// TODO: implement pause functionality

	sMovement();
	sLifespan();
	sCollision();
	sAnimation();
	sRender();
}

void Scene_Play::sMovement()
{
	// TODO: Implement player movement / jumping based on its CInput component
	// TODO: Implement gravity's effect on the player
	// TODO: Implement the maximum player speed in both X and Y directions
	// NOTE: Setting an entity's scale.x to -1/1 will make it face to the left/right
}

void Scene_Play::sLifespan()
{
	// TODO: Check lifespan of entities that have them, and destroy them if they go over
}

void Scene_Play::sCollision()
{
	// REMEMEBER: SFML'S (0,0) position is on the TOP-LEFT corner
	//			  This means jumping will have a negative y-component
	//			  and gravity will have a positive y-component
	//			  Also, something BELOW something else will have a y value GREATE than it
	//			  Also, something ABOVE something ele will have a y value LESS than it

	// TODO: Implement Physics::GetOverlap() function, use it inside this function

	// TODO: Implement bullet / tile collisions
	//		 Destroy the tile if it has a Brick animation
	// TODO: Implement player / tile collisions and resolutions
	//		 Update the CState component of the player to store whether
	//		 it is currently  on the ground or in the air. This will be
	//		 used by the Animation system
	// TODO: Check to see if the player has fallen down a hole (y > height())

	auto& tiles = m_entityManager.getEntities("tile");
	auto& bullets = m_entityManager.getEntities("bullet");

	auto& pTransform = m_player->getComponent<CTransform>();
	// don't know name of this variable
	auto& pBox = m_player->getComponent<CTransform>();

	// player / collision
	for (auto bullet : bullets)
	{
		for (auto tile : tiles)
		{

		}
	}
}

void Scene_Play::sAnimation()
{
	// TODO: Complete the Animation class code first

	// TODO: Set the animation of the player based on its CState component
	// TODO: For each entity with an animation, call entity->getComponent<CAnimation>().animation.update()
	//		 If the animation is not repeated, and it has ended, destroy the entity

	void Scene_Play::onEnd()
	{
		// TODO: When the scene ends, change back to the MENU scene
		//		 use m_game->changeScene(correct params);
	}

	void Scene_Play::sRender()
	{
		// color the background darker so you know the game is paused
		if (!m_paused) { m_game->window().clear(sf:Color(100, 100, 255)); }
		else { m_game->window().clear(sf::Color(50, 50, 150)); }

		// set the viewport of the window to be centered on the player if it's far enough right
		auto& pPos = m_player->getComponent<CTransform>().pos;
		float windowCenterX = std::max(m_game->window().getSize().x / 2.0f, pPos.x);
		sf::View view = m_game->window().getView();
		view.setCenter(windowCenterX, m_game->window().getSize().y - view.getCenter().y);
		m_game->window().setView(view);

		// draw all Entity textures / animations
		if (m_drawTextures)
		{
			for (auto e : m_entityManager.getEntities())
			{
				auto& transform = e->getComponent<CTranform>();

				if (e->hasComponent<CAnimation>())
				{
					auto& animation = e->getComponent<CAnimation>().animation;
					animation.getSprite().setRotation(transform.angle);
					animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
					animation.getSprite().setScale(transform.scale.x, transform.scale.y);
					m_game->window().draw(animation.getSprite());
				}
			}
		}

		// draw all Entity collision bounding boxes wit a rectangleshape
		if (m_drawCollision)
		{
			for (auto e : m_entityManager.getEntities())

				if (e->hasComponent<CBoundingBox>())
				{
					auto& box = e->getComponent<CBoundingBox>();
					auto& transform = e->getComponent<CTransform>();
					sf::RectangleShape rect;
					rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
					rect.setOrigin(sf:Vector2f(box.halfSize.x, box.halfSize.y));
					rect.setPosition(transform.pos.x, transform.pos.y);
					rect.setFillColor(sf:Color(0, 0, 0, 0));
					rect.setOutlineColor(sf::Color(255, 255, 255, 255));
					rect.setOutlineThickness(1);
					m_game->window().draw(rect);
				}
		}
	}

	// draw the grid so that students can easily debug
	if (m_drawGrid)
	{
		float leftX = m_game->window().getCenter().x - width() / 2;
		float rightX = leftX + width() + m_gridSize.x;
		float nextGridX = leftX = ((int)leftX % (int)m_gridSize.x);

		for (float x = nextGridX; x < rightX; x += m_gridSize.x)
		{
			drawLine(Vec2(x, 0), Vec2(x, height()));
		}

		for (float y = 0; y < height(); y += m_gridSize.y)
		{
			drawLine(Vec2(leftX, height() - y), Vec2(rightX, height() - y));

			for (float x = nextGridX; x < rightX; x += m_gridSize.x)
			{
				std::string xCell = std::to_string(int)x / (int)m_gridSize.x);
				std::string yCell = std::to_string(int)y / (int)m_gridSize.y);
				m_gridText.setString("(" + xCell + "," + yCell + ")");
				m_gridText.setPosition(x + 3, height() - y - m_gridSize.y + 2);
				m_game->window().draw(m_gridText);
			}
		}

		// ...






		// don't let the player exit the screen to the left
		// ...
	}
}



void Scene_Play::sDoAction(const Action& action)
{
	if (action.name() == "MOUSE_MOVE")
	{
		m_mousePos = action.pos();
	}

	if (action.type() == "START")
	{
		auto& pInput = m_player->getComponent<CInput>();
		auto& pState = m_player->getComponent<CState>();
		auto& pTransform = m_player->getComponent<CTransform>();

		if (action.name() == "JUMP")
		{
			// don't know if this says cancel or something else
			if (pState.state != "air" && m_player->getComponent<CInput>().cancel)
			{
				pTransform.velocity.y = m.playerConfig.JUMP;
			}
			m_player->getComponent<CInput>().canJump = false;
			m_player->getComponent<CInput>().up = true;
		}
		else if (action.name() == "LEFT") { pInput.left = true; }
		else if (action.name() == "DOWN") { pInput.down = true; }
		else if (action.name() == "RIGHT") { pInput.right = true; }
		else if (action.name() == "SHOOT") { pInput.shoot = true; }
		else if (action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures; }
		else if (action.name() == "TOGGLE_COLLISION") { m_drawCollision = !mdrawCollision; }
		else if (action.name() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid; }
		else if (action.name() == "PAUSE") { setPaused(!m_paused); }
		else if (action.name() == "QUIT") { onEnd(); }




		else if (action.name() == "LEFT_CLICK")
		{
			for (auto e : m_entityManager.getEntities())
			{
				Vec2 wPos = windowToWorld(m_mousePos);
				if (IsInside(wPos, e))
				{

				}
			}

			std::cout << action.toString() << "\n";
		}
	}
}


// ...

void Scene_Play::sRender()
{
	if (!m_paused) { m_game->window().clear(sf::Color(100, 100, 255)); }
	else { m_game->window().clear(sf::Color(50, 50, 150)); }

	// set the viewport of the window to be centered on the player if it's far
	auto& pPos = m_playe->getComponent<CTransform>().pos;
	float windowCenterX = fmax(m_game->window().getSize().x / 2.0f, pPos.x);
	sf::View view = m_game->window().getView();
	// getCenter is incomplete
	view.setCenter(windowCenterX, m_game->window().getSize().y - view.getCenter);
	m_game->window().setView(view);

	sf::CircleShape dot;
	dot.setFillColor(sf::Color::Red);
	dot.setRadius(8);
	dot.setOrigin(8, 8);
	dot.setPosition(m_mousePos.x, m_mousePos.y);
	m_game->window().draw(dot);

	// draw all Entity textures / animation
	if (m_drawTextures)
	{
		for (auto e : m_entityManager.getEntities())
		{
			auto& transform = e->getComponent<CTransform>();

			if (e->hasComponent<CAnimation>())
			{
				auto& animation = e->getComponent<CAnimiation>().animation;
				animatin.getSprite().setRotation(transform.angle);
				animation.getSprite().setPosition(transform.pos.x, transform.pos.y); j
			}
		}
	}
}