#include "resource_manager.hpp"

std::map<std::string, std::weak_ptr<sf::Texture>>& ResourceManager::getMap(sf::Texture*)
{
    return textures_;
}

std::map<std::string, std::weak_ptr<sf::SoundBuffer>>& ResourceManager::getMap(sf::SoundBuffer*)
{
    return soundBuffers_;
}

std::map<std::string, std::weak_ptr<sf::Font>>& ResourceManager::getMap(sf::Font*)
{
    return fonts_;
}