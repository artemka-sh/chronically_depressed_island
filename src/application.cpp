#include "application.hpp"

Application::Application(uint x_res, uint y_res, std::string window_name, uint framerate):
window_(sf::VideoMode({x_res, y_res}), window_name),
cameraman_(dt_),
loader_(),
renderer_(window_, loader_, cameraman_),
input_(window_)
{
    
    window_.setFramerateLimit(framerate);
    
}

int Application::operator()()
{
    run();
    return 0;
}

void Application::run()
{
    while (window_.isOpen())
    {
        dt_ = dtClock_.restart().asSeconds();
        
        renderer_.render();
        input_.process(cameraman_);


        static sf::Clock fpsClock;
        static int fpsFrameCount = 0;
        static float fps = 0.0f;
        static float fpsUpdateTime = 0.0f;
        fpsFrameCount++;
        fpsUpdateTime += fpsClock.restart().asSeconds();
        if (fpsUpdateTime >= 1.0f) {
            fps = fpsFrameCount / fpsUpdateTime;
            fpsFrameCount = 0;
            fpsUpdateTime = 0.0f;
        }

        std::print("fps: {}", fps);
    }
}
