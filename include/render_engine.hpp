#pragma once
#include <SFML/Graphics.hpp>
#include "object_loader.hpp"
#include <print>
#include <cmath>

class RenderEngine{
public:
    RenderEngine() = default;
    RenderEngine(sf::RenderWindow& window, Object_loader& loader);
    //void render(const Scene& scene);
    void render();

    // void renderMesh(const Mesh& mesh, const Transform& transform, 
    //                 const Camera& camera, const Material& material);
    // void applyLighting(const Scene& scene);
    // void postProcess();

    
    private: 
    sf::RenderWindow& window_;
    Object_loader& loader_;
    float focalLength;
    float moveSpeed = 10;

    sf::Vector3f camera_position{0.0f, 0.0f, 0.0f};
    float camera_yaw = 0.0f;   
    float camera_pitch = 0.0f;

    // Для FPS
    sf::Clock fpsClock;
    int fpsFrameCount = 0;
    float fps = 0.0f;
    float fpsUpdateTime = 0.0f;

    int SCR_X;
    int SCR_Y;
    
};


