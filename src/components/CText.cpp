#include "CText.h"
#include "../AssetManager.h"


CText::CText() {
	loadFromAssetManager();
}

CText::CText(const std::string& name) : name(name) {
	loadFromAssetManager();
}

bool CText::loadFromAssetManager() {
	auto& assetManager = AssetManager::GetInstance();
	try {
		font = assetManager.GetFont(name);
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Failed to load font: " << name << " with error: " << e.what() << std::endl;
		return false;
	}
}

bool CText::loadFromAssetManager(const std::string& textureName) {
	auto& assetManager = AssetManager::GetInstance();
	try {
		font = assetManager.GetFont(name);
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Failed to load font: " << name << " with error: " << e.what() << std::endl;
		return false;
	}
}