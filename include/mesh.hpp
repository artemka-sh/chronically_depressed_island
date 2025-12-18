#pragma once
#include <SFML/System.hpp>

class Mesh
{
private:
    /* data */
public:
    Mesh() = default;
    Mesh(std::vector<sf::Vector3f> dots, std::vector<std::vector<int>> faces);
    ~Mesh() = default;
    std::vector<sf::Vector3f> dots_;
    std::vector<std::vector<int>> faces_; 
};

