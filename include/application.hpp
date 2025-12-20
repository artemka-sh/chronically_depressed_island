#pragma once
#include <SFML/Graphics.hpp>
#include "render_engine.hpp"
#include "cameraman.hpp"
#include "input.hpp"

class Application {
    sf::RenderWindow window_;
    Cameraman cameraman_;
    Object_loader loader_;
    RenderEngine renderer_;
    Input input_;
    float dt_;
    sf::Clock dtClock_;
    
public:
    Application(uint x_res, uint y_res, std::string window_name, uint framerate = 60);
    int operator()();
    void run();
    void handleEvents();
    void update(float dt);
    void render();
};
