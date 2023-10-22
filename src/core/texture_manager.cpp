#include "texture_manager.h"

TextureManager::TextureManager() = default;

void TextureManager::addTexture(const std::string &id, const std::string &texturePath) {
    auto it = textureMap_.find(id);
    if (it != textureMap_.end()) {
        std::string msg = "Unable to add texture: '" + id + "' already exists\n";
        throw new std::runtime_error(msg);
    }

    std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();

    if (!texture->loadFromFile(texturePath)) {
        std::string msg = "Unable to open texture '" + texturePath + "'\n";
        throw new std::runtime_error(msg);
    }

    texture->setSmooth(true);

    textureMap_.emplace(id, texture);

    std::cout << "Texture with path: [" << texturePath << "] has been added\n";
}

std::shared_ptr<sf::Texture> TextureManager::getTexture(const std::string &id) {
    auto it = textureMap_.find(id);
    if (it == textureMap_.end()) {
        std::cout << "Unable to load texture: " << id << " doesn't exist\n";
        return nullptr;
    }

    return it->second;
}

bool TextureManager::hasTexture(const std::string& id)
{
    return textureMap_.contains(id);
}
