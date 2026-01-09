#ifndef ISLAND_CAMERAMAN_H
#define ISLAND_CAMERAMAN_H
#include <SFML/Graphics.hpp>
#include <print>
#include <cmath>

class RenderEngine;
class Input;
class Cameraman
{
    friend class RenderEngine;
    friend class Input;

    enum class Direction {
        Forward,
        Backward,
        Left,
        Right,
        Up,
        Down
    };

    enum class Action {
        PrintDebug,
        Jump,
        Use
    };


    sf::Vector3f position_{0.0f, 0.0f, 0.0f};
    //sf::Vector3f moveVector;
    float FOV_ = 90.0f;
    float yaw_ = 0.0f;
    float pitch_ = 0.0f;
    float moveSpeed_ = 10.0f;
    //float rotationSpeed_ = 0.02f;


public:
    explicit Cameraman() = default;
    void printDebugInfo() const;
    void move(Direction direction);
    void cameraRotate(Direction direction);
    void justdo(Action action);
};


#endif //ISLAND_CAMERAMAN_H