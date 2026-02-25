#pragma once
#include <vector>
#include "mesh.hpp"
#include <memory>
#include <fstream>
#include <SFML/System.hpp>
#include <print>
#include "resource_manager.hpp"
// #include <iostream>


class Object_loader
{
private:
    std::vector<std::shared_ptr<Mesh>> objects;
    ResourceManager& resourceManager_;
public:
    Object_loader(ResourceManager& resource_manager);
    ~Object_loader();
    void load_3d_data(std::shared_ptr<Mesh> mesh, const std::string& path);

    std::shared_ptr<Mesh> load_mesh(std::string model_path);
    std::shared_ptr<Mesh> load_mesh();
    std::vector<std::shared_ptr<Mesh>>& getMeshVec();
};

