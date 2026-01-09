#include "cameraman.hpp"

void Cameraman::printDebugInfo() const
{
    const sf::Vector3f& c = position_;
    std::print("Camera position: {}, {}, {}\n", c.x, c.y, c.z);
}

void Cameraman::move(Direction dir)
{
    switch(dir) {
    case Direction::Forward:
        position_.z += moveSpeed_ * cos(yaw_);
        position_.x += moveSpeed_ * sin(yaw_);
        break;
    case Direction::Backward:
        position_.z -= moveSpeed_ * cos(yaw_);
        position_.x -= moveSpeed_ * sin(yaw_);
        break;
    case Direction::Left:
        position_.x -= moveSpeed_ * cos(yaw_);
        position_.z += moveSpeed_ * sin(yaw_);
        break;
    case Direction::Right:
        position_.x += moveSpeed_ * cos(yaw_);
        position_.z -= moveSpeed_ * sin(yaw_);
        break;

    case Direction::Up:
        position_.y += moveSpeed_;
        break;
    case Direction::Down:
        position_.y -= moveSpeed_;
        break;
    }
}

void Cameraman::cameraRotate(Direction dir)
{
    switch(dir) {

    case Direction::Left:
        yaw_ -= 0.02f;
        break;
    case Direction::Right:
        yaw_ += 0.02f;
        break;
    case Direction::Up:
        pitch_ += 0.02f;
        break;
    case Direction::Down:
        pitch_ -= 0.02f;
        break;
    }
}

void Cameraman::justdo(Action action)
{
    switch (action)
    {
    case Action::PrintDebug:
        std::print("Debug info: \n");
        std::print("Camera position is: {}, {}, {}\n", position_.x, position_.y, position_.z);
        break;
    }
}
