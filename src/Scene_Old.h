#pragma once
#include "EntityManager.h"

#include <memory>

class Scene_Old
{

protected:
	EntityManager&	entity_manager_ = EntityManager::GetInstance();
	bool			paused_ = false;
	bool			has_ended_ = false;
	size_t			current_frame_ = 0;

	
	void setPaused(bool paused);

public:
	virtual void onEnd() = 0;
	Scene_Old();

	virtual void update() = 0;
	virtual void sRender() = 0;
	virtual void sCollision() = 0;
};