#include "../Action.h"
#include <iostream>
#include <string>   
#include <sstream>  

Action::Action() { }

Action::Action(const std::string& name, const std::string& type)
	: name_(name), type_(type) { }

Action::Action(const std::string& name, Vec2 pos)
	: name_(name), pos_(pos) { }  // Assuming you want to initialize name_ and pos_ here

Action::Action(const std::string& name, const std::string& type, Vec2 pos)
	: name_(name), type_(type), pos_(pos) { }

const std::string& Action::name() const {
	return name_;
}

const std::string& Action::type() const {
	return type_;
}

const Vec2& Action::pos() const {
	return pos_;
}

std::string Action::toString() const {
	std::stringstream ss;
	ss << name() << " " << type() << " " << static_cast<int>(pos().x) << " " << static_cast<int>(pos().y);
	return ss.str();
}
