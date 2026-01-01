#ifndef ISLAND_RESOURCE_MANAGER_H
#define ISLAND_RESOURCE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <memory>
#include <string>
#include <concepts>
#include <print>


template <typename T>
concept LoadableFromFile = requires(T t, const std::string& filename)
{
    { t.loadFromFile(filename) } -> std::convertible_to<bool>;
};

class ResourceManager
{
public:
    ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    template <LoadableFromFile T>
    std::shared_ptr<T> get(const std::string& filename) {
        auto& targetMap = this->getMap(static_cast<T*>(nullptr));

        if (auto it = targetMap.find(filename); it != targetMap.end()) {
            if (auto shared = it->second.lock()) {
                return shared;
            }
            targetMap.erase(it);
        }

        auto resource = std::make_shared<T>();
        if (!resource->loadFromFile(filename)) {
            std::println(stderr, "Error: Failed to load resource: {}", filename);
            throw std::runtime_error("stop.");
            return nullptr;
        }

        targetMap[filename] = resource;
        std::println("Debug: Loaded new resource: {}", filename);
        return resource;
    }

private:
    std::map<std::string, std::weak_ptr<sf::Texture>> textures_;
    std::map<std::string, std::weak_ptr<sf::SoundBuffer>> soundBuffers_;
    std::map<std::string, std::weak_ptr<sf::Font>> fonts_;

    std::map<std::string, std::weak_ptr<sf::Texture>>& getMap(sf::Texture*);
    std::map<std::string, std::weak_ptr<sf::SoundBuffer>>& getMap(sf::SoundBuffer*);
    std::map<std::string, std::weak_ptr<sf::Font>>& getMap(sf::Font*);
};

#endif //ISLAND_RESOURCE_MANAGER_H