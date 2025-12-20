# pragma once
#include "SFML/Graphics.hpp"
#include "cameraman.hpp"
#include <print>
#include <map>

class Input
{
sf::RenderWindow& window_;
    static const std::map<sf::Keyboard::Key, Cameraman::Direction> moveKeymap;
    static const std::map<sf::Keyboard::Key, Cameraman::Direction> rotateKeymap;
public:
    Input(sf::RenderWindow& window);
    void process(Cameraman &cameraman);

};




