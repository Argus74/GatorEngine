#include "AssetManager.h"

#include <filesystem>
#include <iostream>

#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Texture.hpp"

AssetManager& AssetManager::GetInstance() {
    static AssetManager instance;
    return instance;
}

AssetManager::AssetManager() {
    //Intializing all png files as textures in Start & Internal Assets folder
    IntializeAssets("assets/StartAssets", false);
    IntializeAssets("assets/InternalAssets", true);

    Animation ani = Animation("DefaultAnimation", GetTexture("DefaultAnimation"), 11, 1);
    AddAnimation("DefaultAnimation", ani);
    Animation ani2 = Animation("RunningAnimation", GetTexture("RunningAnimation"), 12, 1);
    AddAnimation("RunningAnimation", ani2);
}

AssetManager::~AssetManager() {
    // Deleting textures
    for (auto& pair : textures_) {
        delete pair.second;
    }
    textures_.clear();  //Removing all elements from the map

    for (auto& pair : sounds_) {  // Deleting sounds

        delete pair.second;
    }
    sounds_.clear();

    // Deleting fonts
    for (auto& pair : fonts_) {
        delete pair.second;
    }
    fonts_.clear();

    for (auto& pair : animations_) {
        delete pair.second;
    }
    animations_.clear();

    for (auto& pair : game_engine_textures_) {
        delete pair.second;
    }
    game_engine_textures_.clear();
}

void AssetManager::AddTexture(const std::string& name, const std::string& path) {
    sf::Texture* texture = new sf::Texture();
    if (!texture->loadFromFile(path)) {
        std::cout
            << "Failed to load texture: " << path
            << std::
                   endl;  // For now just using the standard error stream to display the errors, Later on we should change this to output to our error console we create
        
    }

    textures_[name] = texture;
    texture_paths_[name] = path;
}

void AssetManager::AddTexturePrivate(const std::string& name, const std::string& path) {
    sf::Texture* texture = new sf::Texture();
    if (!texture->loadFromFile(path)) {
        // For now just using the standard error stream to display the errors, Later on we should change this to output to our error console we create
        std::cout << "Failed to load texture: " << path << std::endl;  
        delete texture;
        return;
    }
    game_engine_textures_[name] = texture;
}

void AssetManager::AddSound(const std::string& name, const std::string& path) {
    sf::SoundBuffer* buffer = new sf::SoundBuffer();
    if (!buffer->loadFromFile(path)) {
        std::cerr << "Failed to load sound buffer: " << path << std::endl;
        delete buffer;
        return;
    }
    sounds_[name] = buffer;
}

void AssetManager::AddFont(const std::string& name, const std::string& path) {
    sf::Font* font = new sf::Font();
    if (!font->loadFromFile(path)) {
        std::cerr << "Failed to load font: " << path << std::endl;
        delete font;
        return;
    }
    fonts_[name] = font;
}

void AssetManager::AddAnimation(const std::string& name, const Animation& animation) {
    Animation* ani = new Animation(animation);
    animations_[name] = ani;
}

void AssetManager::IntializeAssets(std::string path, bool makePrivate) {
    // Adding all assets that are in Start Assets
    std::cout << "Checking path: " << std::filesystem::absolute(path) << std::endl;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
        // Check if the entry is a file with a ".png" extension
        if (entry.is_regular_file() && entry.path().extension() == ".png") {
            // Extract the file name without extension to use as a texture name
            std::string textureName = entry.path().stem().string();
            std::cout << textureName << std::endl;
            // Add the texture to the asset manager
            if (makePrivate) {
                AddTexturePrivate(textureName, entry.path().string());
            } else {
                AddTexture(textureName, entry.path().string());
            }
        } else if (entry.is_regular_file() && entry.path().extension() == ".ttf") {
            // Extract the file name without extension to use as a font name
            std::string fontName = entry.path().stem().string();
            std::cout << fontName << std::endl;

            // Add the font to the asset manager
            AddFont(fontName, entry.path().string());
        }
    }
}

sf::Sound AssetManager::PlaySound(
    const std::string& name) {  //Function that plays sounds from our map of SoundBuffers
    if (sounds_.find(name) == sounds_.end()) {
        throw std::runtime_error("Sound buffer not found: " + name);
    }
    sf::Sound sound;
    sound.setBuffer(*sounds_[name]);
    sound.play();
    return sound;
}

sf::Texture& AssetManager::GetTexture(const std::string& name) {
    if (textures_.find(name) == textures_.end()) {
        std::cout << "Texture not found: " << name << std::endl;
        return *game_engine_textures_["ErrorOut"];
    }
    return *textures_[name];
}

sf::Texture& AssetManager::GetTexturePrivate(const std::string& name) {
    if (game_engine_textures_.find(name) == game_engine_textures_.end()) {
        std::cerr << "Texture not found: " << name << std::endl;
        throw std::runtime_error("Texture not found");
    }
    return *game_engine_textures_[name];
}

sf::SoundBuffer& AssetManager::GetSound(const std::string& name) {
    if (sounds_.find(name) == sounds_.end()) {
        std::cerr << "Sound not found: " << name << std::endl;
        throw std::runtime_error("Sound not found");
    }
    return *sounds_[name];
}

sf::Font& AssetManager::GetFont(const std::string& name) {
    if (fonts_.find(name) == fonts_.end()) {
        std::cout << "Font not found: " << name << std::endl;
        return *fonts_["MontserratBlack"];
    }
    return *fonts_[name];
}

Animation& AssetManager::GetAnimation(const std::string& name) {
    if (animations_.find(name) == animations_.end()) {
        std::cout << "Animation not found: " << name << std::endl;
        throw std::runtime_error("Animation not found");
    }
    return *animations_[name];
}

sf::Color AssetManager::LerpColor(const sf::Color& start, const sf::Color& end, float t) {
    return sf::Color(static_cast<sf::Uint8>(start.r + t * (end.r - start.r)),
                     static_cast<sf::Uint8>(start.g + t * (end.g - start.g)),
                     static_cast<sf::Uint8>(start.b + t * (end.b - start.b)),
                     255  // Assuming full opacity
    );
}