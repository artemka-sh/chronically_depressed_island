#ifndef ISLAND_CAMERA_INTERFACE_H
#define ISLAND_CAMERA_INTERFACE_H

#include <SFML/Graphics.hpp>

struct Camera_state
{
    sf::Vector3f position;
    sf::Vector3f moveSpeed;
    float pitch;
    float yaw;
};
#endif //ISLAND_CAMERA_INTERFACE_H