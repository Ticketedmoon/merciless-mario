#pragma once

#ifndef VANQUISH_TEXTURE_MANAGER_H
#define VANQUISH_TEXTURE_MANAGER_H

#include <SFML/Graphics.hpp>

#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <filesystem>

class TextureManager
{
    public:
        TextureManager();
        void addTexture(const std::string& id, const std::string& texturePath);
        std::shared_ptr<sf::Texture> getTexture(const std::string& id);
        bool hasTexture(const std::string& id);

    private:
        std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textureMap_;
};

#endif //VANQUISH_TEXTURE_MANAGER_H