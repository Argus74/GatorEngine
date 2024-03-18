#include "Scene.h"

void Scene::setPaused(bool paused)
{
	m_paused = paused;
}

Scene::Scene()
{

}

size_t Scene::currentFrame() const
{
	return m_currentFrame;
}
