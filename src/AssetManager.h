/**
 * File: AssetManager.h
 * Asset Management for Gator Engine
 * 
 * The AssetManager class is a singleton that manages all the resources used by Gator Engine.
 * It is responsible for loading, accessing, and managing assets such as textures, sounds,
 * fonts, and animations. The AssetManager provides a centralized point for accessing these
 * resources, ensuring that each asset is only loaded once and can be reused throughout the game.
 * It also provides utility functions for name extraction from file paths, sound playback, and
 * color interpolation. Private assets, such as icons and buttons specific to Gator Engine's 
 * internal UI, are managed separately to distinguish between game assets and engine assets.
 */

#ifndef ASSETS_H_
#define ASSETS_H_

#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Texture.hpp"

#include "Animation.h"
#include "util/Serializable.h"

class AssetManager : public Serializable {
 public:
    static AssetManager& GetInstance();
    ~AssetManager();
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;
    AssetManager(AssetManager&&) = delete;
    AssetManager& operator=(AssetManager&&) = delete;

    // Asset addition methods
    void AddTexture(const std::string& name, const std::string& path);
    void AddTexturePrivate(
        const std::string& name,
        const std::string&
            path);  //Used for Assets needed for Gator Engine, E.G Icon Buttons, or Sprite Buttons
    void AddSound(const std::string& name, const std::string& path);
    void AddFont(const std::string& name, const std::string& path);
    void AddAnimation(const std::string& name, const Animation& animation);

    // Intialize Start and Internal Assets
    void IntializeAssets(std::string path, bool makePrivate = false);

    //Creating a list of Texture Name pointers, this is necessary to populate the drop down selection list when selecting a sprite
    std::vector<const char*> GenerateAssetNameList(std::string assetType) {

        std::vector<const char*> nameList;

        if (assetType == "textures") {
            // Reserve space in the vector for efficiency.
            nameList.reserve(textures_.size());

            for (const auto& texturePair : textures_) {
                // Add the address of the string key in the map.
                nameList.push_back(texturePair.first.c_str());
            }

            return nameList;
        } else if (assetType == "animations") {
            // Reserve space in the vector for efficiency.
            nameList.reserve(animations_.size());

            for (const auto& animationPair : animations_) {
                // Add the address of the string key in the map.
                nameList.push_back(animationPair.first.c_str());
            }

            return nameList;
        } else if (assetType == "fonts") {
            // Reserve space in the vector for efficiency.
            nameList.reserve(fonts_.size());

            for (const auto& fontPair : fonts_) {
                // Add the address of the string key in the map.
                nameList.push_back(fontPair.first.c_str());
            }

            return nameList;
        } else {
            return nameList;
        }
    };

    // Asset retrieval methods
    sf::Texture& GetTexture(const std::string& name);
    sf::Texture& GetTexturePrivate(
        const std::string&
            name);  //Used for Assets needed for Gator Engine, E.G Icon Buttons, or Sprite Buttons
    sf::SoundBuffer& GetSound(const std::string& name);
    sf::Font& GetFont(const std::string& name);
    Animation& GetAnimation(const std::string& name);

    //Plaing a sound using the stored sounds that are in our map
    sf::Sound PlaySound(const std::string& name);

    // Function that extracts a filename from a filepath
    static std::string ExtractFilename(const std::string& path) {
        // Find the last slash in the path
        size_t lastSlashPos = path.find_last_of("/\\");

        // Extract the filename with extension
        std::string filenameWithExtension = path.substr(lastSlashPos + 1);

        // Find the last dot in the filename
        size_t lastDotPos = filenameWithExtension.find_last_of('.');

        // Extract the filename without the extension
        std::string filenameWithoutExtension = filenameWithExtension.substr(0, lastDotPos);

        return filenameWithoutExtension;
    };

    //Misc function for managing Lerp Colors (Interpolating)
    static sf::Color LerpColor(const sf::Color& colorStart, const sf::Color& colorEnd, float t);

    void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override {
        writer.StartObject();
        writer.Key("animations");
        writer.StartObject();
        for (auto animation : animations_) {
            writer.Key(animation.first.c_str());
            animation.second->serialize(writer);
        }
        writer.EndObject();
        writer.EndObject();
    }

    void deserialize(const rapidjson::Value& value) override {
        for (auto it = value["animations"].MemberBegin(); it != value["animations"].MemberEnd(); it++) {
            Animation animation;
            animation.deserialize(value);
            AddAnimation(it->name.GetString(), Animation(animation.GetName(), GetTexture(animation.GetName()), animation.frame_count, animation.speed));
        }
    }

 private:
    AssetManager();

    std::map<std::string, sf::Texture*> textures_;
    std::map<std::string, sf::Texture*>
        game_engine_textures_;  //Used for Assets needed for Gator Engine, E.G Icon Buttons, or Sprite Buttons
    std::map<std::string, sf::SoundBuffer*>
        sounds_;  //Sounds are stored as a sound buffer, and then played with a function using SMFL::Sound
    std::map<std::string, sf::Font*> fonts_;
    std::map<std::string, Animation*> animations_;
    std::map<std::string, std::string> texture_paths_;
};

#endif  // ASSETS_H_