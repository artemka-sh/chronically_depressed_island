#pragma once
#include <vector>
#include "mesh.hpp"
#include <memory>
#include <fstream>
#include <SFML/System.hpp>
#include <print>
#include <iostream>


class Object_loader
{
private:
    std::vector<std::shared_ptr<Mesh>> objects;
public:
    Object_loader(/* args */);
    ~Object_loader();
    std::shared_ptr<Mesh> load_mesh(std::string model_path);
    std::vector<std::shared_ptr<Mesh>>& getMeshVec();
};

