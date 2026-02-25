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

struct Polygon {
    sf::Vertex vertices[3];
    std::shared_ptr<sf::Texture> texture_;
    float depth_;
};


class RenderEngine
{
public:
    RenderEngine() = default;
    RenderEngine(sf::RenderWindow& window, Object_loader& loader, Cameraman& cameraman);
    //void render(const Scene& scene);
    void render();
    void rendSkybox();

    std::vector<sf::Vector2f> calculateDots(const std::vector<sf::Vector3f>& dots);
    void render3DMesh(const Mesh& mesh);
    void render2DMesh(const Mesh& mesh);
    void renderBatch();
    // void applyLighting(const Scene& scene);
    // void postProcess();


private:
    sf::RenderWindow& window_;
    Object_loader& loader_;
    Cameraman& cameraman_;

    float rFOV;
    float focalLength;

    int SCR_X;
    int SCR_Y;

    std::vector<Polygon> polygonVector;
    // Временные объекты
    std::vector<std::vector<int>> visibleFaces_;
    sf::Vector3f sunLightDirection{0.4f, 0.8f, -0.9f};
    float day = 0.0f;
    float year = 0.0f;

};
