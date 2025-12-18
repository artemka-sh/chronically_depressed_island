#include "application.hpp"

Application::Application(uint x_res, uint y_res, std::string window_name, uint framerate):
window_(sf::VideoMode({x_res, y_res}), window_name),
loader_(),
renderer_(window_, loader_)
{
    
    window_.setFramerateLimit(framerate);
    
}

int Application::operator()()
{
    std::print("here was");
    run(); 
    std::print("here was");
    return 0;
}

void Application::run()
{
    while (window_.isOpen())
    {
        dt = dtClock.restart().asSeconds();
        
        renderer_.render();

    }
}
