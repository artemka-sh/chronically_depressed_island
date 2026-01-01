#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <initializer_list>

class Mesh
{
public:
    enum class Dimens
    {
        _3d,
        _2d,
        _0d
    };

    Mesh() = default;
    Mesh(std::vector<sf::Vector3f> dots, std::vector<std::vector<int>> faces,
         sf::Vector3f position, bool is_static, sf::Vector3f scale, uint8_t lightness, Dimens dimen);
    ~Mesh() = default;
    std::vector<sf::Vector3f> dots_;
    std::vector<std::vector<int>> faces_; //заменить на array или другой контейнер для производительности
    std::shared_ptr<sf::Texture> texture_;
    std::shared_ptr<sf::SoundBuffer> one_sound_;

    sf::Vector3f position_;
    bool isStatic_;
    sf::Vector3f scale_;
    uint8_t lightness_;
    Dimens dimen_;
};

