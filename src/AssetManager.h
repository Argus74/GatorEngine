#ifndef ASSETS_H_
#define ASSETS_H_

#include <map>
#include <string>
#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Font.hpp>
#include <iostream>
#include "Animation.h"
#include <string>

//namespace sf { //forward declaration
//	class Texture;
//	class SoundBuffer;
//	class Sound;
//	class Font;
//} 

class AssetManager {
public:
	static AssetManager& GetInstance();
	~AssetManager();
	AssetManager(const AssetManager&) = delete;
	AssetManager& operator=(const AssetManager&) = delete;
	AssetManager(AssetManager&&) = delete;
	AssetManager& operator=(AssetManager&&) = delete;

	//This list will keep track of which textures are in our asset database, 
	std::vector<const char*> TextureNameList;

	// Asset addition methods 
	void AddTexture(const std::string& name, const std::string& path);
	void AddSound(const std::string& name, const std::string& path);
	void AddFont(const std::string& name, const std::string& path);
	void AddAnimation(const std::string& name, const Animation& animation);

	// Intialize Start Assets
	void IntializeTextureAssets(std::string path);
	//Creating a list of Texture Name pointers, this is necessary to populate the drop down selection list when selecting a sprite
	std::vector<const char*> GenerateTextureNamePointers() {
		std::vector<const char*> textureNamePointers;

		// Reserve space in the vector for efficiency.
		textureNamePointers.reserve(textures_.size());

		for (const auto& texturePair : textures_) {
			// Add the address of the string key in the map.
			textureNamePointers.push_back(texturePair.first.c_str());
		}

		return textureNamePointers;
	};

	// Asset retrieval methods 
	sf::Texture& GetTexture(const std::string& name);
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

private:
	AssetManager();

	std::map<std::string, sf::Texture*> textures_;
	std::map<std::string, sf::SoundBuffer*> sounds_; //Sounds are stored as a sound buffer, and then played with a function using SMFL::Sound
	std::map<std::string, sf::Font*> fonts_;
	std::map<std::string, Animation*> animations_;
};


#endif  // ASSETS_H_