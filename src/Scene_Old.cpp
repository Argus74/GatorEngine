#include "Scene_Old.h"

void Scene_Old::setPaused(bool paused)
{
	paused_ = paused;
}

Scene_Old::Scene_Old()
{

}

size_t Scene_Old::currentFrame() const
{
	return current_frame_;
}
