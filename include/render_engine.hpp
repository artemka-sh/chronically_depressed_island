#pragma once
#include <SFML/Graphics.hpp>
#include "object_loader.hpp"
#include "cameraman.hpp"
#include <print>
#include <cmath>
#include <map>
#include <ranges>
#include <vector>
#include <algorithm>

class RenderEngine
{
public:
    RenderEngine() = default;
    RenderEngine(sf::RenderWindow& window, Object_loader& loader, Cameraman& cameraman);
    //void render(const Scene& scene);
    void render();

    // void renderMesh(const Mesh& mesh, const Transform& transform,
    //                 const Camera& camera, const Material& material);
    // void applyLighting(const Scene& scene);
    // void postProcess();


private:
    sf::RenderWindow& window_;
    Object_loader& loader_;
    Cameraman& cameraman_;

    int SCR_X;
    int SCR_Y;

    // Временные объекты
    std::vector<std::vector<int>> visibleFaces_;
    sf::Vector3f sunLightDirection{0.4f, 0.8f, -0.9f};
};
