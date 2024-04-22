#pragma once

#include <string>

#include "Entity.h"

class Action {
    std::string name_ = "NONE";
    std::string type_ = "NONE";
    Vec2 pos_ = Vec2(0, 0);

 public:
    Action();
    Action(const std::string& name, const std::string& type);
    Action(const std::string& name, Vec2 pos);
    Action(const std::string& name, const std::string& type, Vec2 pos);

    const std::string& name() const;
    const std::string& type() const;
    const Vec2& pos() const;

    std::string toString() const;
};
