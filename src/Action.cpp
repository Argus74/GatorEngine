#include "Action.h"
#include <iostream>
#include <string>   
#include <sstream>  


Action::Action() { }

Action::Action(const std::string& name, const std::string& type)
	: m_name(name), m_type(type) { }

Action::Action(const std::string& name, Vec2 pos)
	: m_name(name), m_pos(pos) { }  // Assuming you want to initialize m_name and m_pos here

Action::Action(const std::string& name, const std::string& type, Vec2 pos)
	: m_name(name), m_type(type), m_pos(pos) { }

const std::string& Action::name() const {
	return m_name;
}

const std::string& Action::type() const {
	return m_type;
}

const Vec2& Action::pos() const {
	return m_pos;
}

std::string Action::toString() const {
	std::stringstream ss;
	ss << name() << " " << type() << " " << static_cast<int>(pos().x) << " " << static_cast<int>(pos().y);
	return ss.str();
}
