#include "application.hpp"

Application::Application(uint x_res, uint y_res, std::string window_name, uint framerate)
{
    window.create(sf::VideoMode({x_res, y_res}), window_name);
    window.setFramerateLimit(framerate);
}

void Application::run()
{
    
}