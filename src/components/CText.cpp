#include "CText.h"
#include "../AssetManager.h"


CText::CText() {
	loadFromAssetManager();
}

CText::CText(const std::string& name) : name_(name) {
	loadFromAssetManager();
}

bool CText::loadFromAssetManager() {
	auto& assetManager = AssetManager::GetInstance();
	try {
		font_ = assetManager.GetFont(name_);
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Failed to load font: " << name_ << " with error: " << e.what() << std::endl;
		return false;
	}
}

bool CText::loadFromAssetManager(const std::string& textureName) {
	auto& assetManager = AssetManager::GetInstance();
	try {
		font_ = assetManager.GetFont(name_);
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Failed to load font: " << name_ << " with error: " << e.what() << std::endl;
		return false;
	}
}