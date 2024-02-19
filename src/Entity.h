#pragma once
#include "Components.h"
#include <tuple>
#include <string>
#include <vector>
class EntityManager;

typedef std::tuple<
    CTransform,
    CShape
> ComponentTuple

class Entity
{
    friend class EntityManager;
    std::tuple<C1, C2, C3, C4> m_components;
    bool m_active = true;
    std::string m_tag = "default";
    size_t m_id = 0;
    ComponentTuple m_components;
    Entity* Parent = nullptr;
    std::vector<Entity*> Children;

    Entity(const size_t& id, const std::string& tag) {}
public:
    
    void destroy();
    size_t id() const;
    bool isActive() const;
    const std::string& tag() const;

    template <typename T>
    bool hasComponent() const {
        return getComponent<T>().has;
    }

    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs) {
        auto& component = getComponent<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.has = true;
        return component;
    }

    template<typename T>
    
    const T& getComponent<T>() const {
        return std::get<T>(m_components);
    }

    template<typename T>
    void removeComponent() {
        getComponent<T>() = T();
    }
}