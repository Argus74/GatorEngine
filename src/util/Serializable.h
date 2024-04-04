#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "../../extern/rapidjson/document.h"
#include "../../extern/rapidjson/writer.h"
#include "../../extern/rapidjson/stringbuffer.h"

class Serializable {
public:
    virtual void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) = 0;
    virtual void deserialize(const rapidjson::Value& value) = 0;

    // Function to read data from a JSON file
    bool readFromJSONFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Failed to open file for reading: " << filename << std::endl;
            return false;
        }

        std::string jsonString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        rapidjson::Document document;
        document.Parse(jsonString.c_str());

        deserialize(document);

        file.close();
        return true;
    }

    // Function to write data to a JSON file
    bool writeToJSONFile(const std::string& filename) {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        serialize(writer);

        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Failed to open file for writing: " << filename << std::endl;
            return false;
        }

        file << buffer.GetString() << std::endl;
        file.close();
        return true;
    }
};