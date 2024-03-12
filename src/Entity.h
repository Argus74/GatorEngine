#pragma once

#include "Components.h"

#include <tuple>
#include <string>

class EntityManager;

typedef std::tuple<
	CTransform,
	CLifeSpan,
	CInput,
	CBoundingBox,
	CAnimation,
	CGravity,
	CState>
	Component Tuple;

class Entity
{
	friend class EntityManager;

	bool			m_active = true;
	std::string		m_tag = "default";
	size_t			m_id = 0;
	ComponentTuple	m_components;

	// constructor is private so we can never create
	// entities outside the EntityManager which had friend access
	Entity(cons size_t& id, const std::string& tag);

public:
	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CName> cName;
	std::shared_ptr<CShape> cShape;
	std::shared_ptr<CBBox> cBBox;

	void				destroy();
	size_t				id() const;
	bool				isActive() const;
	const std::string& tag() const;

	template <typename T>
	bool hasComponent() const
	{
		return getComponent<T>().has;
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		auto& component = getComponent<T>();
		component = T(std::forward<TArgs>(mArgs)...);
		component.has = true;
		return component;
	}

	template<typename T>
	T& getComponent()
	{
		return std::get<T>(m_components);
	}

	template<typename T>
	const T& getComponent() const
	{
		return std::get<T>(m_components);
	}

	template<typename T>
	void removeComponent()
	{
		getComponenent<T>() = T();
	}
};

#endif // ENTITY_H