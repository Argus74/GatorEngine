/*
* File:  CTouchTrigger
*
* eeeeeeee
*/

#ifndef TOUCH_TRIGGER_H
#define TOUCH_TRIGGER_H

#include "../ActionBus.h"
#include "Component.h"

class CTouchTrigger : public Component {
public:
	DECLARE_COMPONENT_NAME("Touch Trigger");
	std::map<std::string, Action> tag_map;
	CTouchTrigger() {}

	void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override {
		writer.StartObject();
		writer.Key("tagMap");
		writer.StartObject();
		for (const auto& pair : tag_map) {
			writer.Key(pair.first.c_str());
			writer.Int(pair.second);
		} 
		writer.EndObject();
		writer.EndObject();
	}

	void deserialize(const rapidjson::Value& value) override {
		for (auto it = value["tagMap"].MemberBegin(); it != value["tagMap"].MemberEnd(); ++it) {
  	        if (it->value.IsInt()) {
  	            tag_map[it->name.GetString()] = static_cast<Action>(it->value.GetInt());
            }
        }
	}
};

#endif // TOUCH_TRIGGER_H