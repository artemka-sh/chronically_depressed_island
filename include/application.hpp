#pragma once
#include <SFML/Graphics.hpp>
#include "render_engine.hpp"

class Application {
    sf::RenderWindow window;
    // Scene scene;
    RenderEngine renderer;
    // Input input;
    
public:
    Application(uint x_res, uint y_res, std::string window_name, uint framerate = 60);
    void run();
    void handleEvents();
    void update();
    void render();
};
