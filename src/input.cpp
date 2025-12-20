#include "input.hpp"

const std::map<sf::Keyboard::Key, Cameraman::Direction> Input::moveKeymap =
{
    {sf::Keyboard::Key::W, Cameraman::Direction::Forward},
    {sf::Keyboard::Key::S, Cameraman::Direction::Backward},
    {sf::Keyboard::Key::A, Cameraman::Direction::Left},
    {sf::Keyboard::Key::D, Cameraman::Direction::Right},
    {sf::Keyboard::Key::Q, Cameraman::Direction::Down},
    {sf::Keyboard::Key::E, Cameraman::Direction::Up}
};

const std::map<sf::Keyboard::Key, Cameraman::Direction> Input::rotateKeymap =
{
    {sf::Keyboard::Key::Left,  Cameraman::Direction::Left},
    {sf::Keyboard::Key::Right, Cameraman::Direction::Right},
    {sf::Keyboard::Key::Up,    Cameraman::Direction::Up},
    {sf::Keyboard::Key::Down,  Cameraman::Direction::Down}
};

Input::Input(sf::RenderWindow& window): window_(window)
{

}
void Input::process(Cameraman &cameraman)
{
    while (const std::optional event = window_.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window_.close();
        }
    }

    // Обработка движения
    for (const auto& [key, direction] : moveKeymap)
    {
        if (sf::Keyboard::isKeyPressed(key))
        {
            cameraman.move(direction/*, dt*/);
        }
    }

    // Обработка поворота
    for (const auto& [key, direction] : rotateKeymap)
    {
        if (sf::Keyboard::isKeyPressed(key))
        {
            cameraman.cameraRotate(direction/*, dt*/);
        }
    }


}