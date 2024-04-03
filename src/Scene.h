#pragma once
#include "EntityManager.h"

#include <memory>

class Scene
{

protected:
	EntityManager&	m_entityManager = EntityManager::GetInstance();
	bool			m_paused = false;
	bool			m_hasEnded = false;

	
	void setPaused(bool paused);

public:
	virtual void onEnd() = 0;
	Scene();

	virtual void update() = 0;
	virtual void sRender() = 0;
	virtual void sCollision() = 0;
};