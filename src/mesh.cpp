#include "mesh.hpp"

Mesh::Mesh(std::vector<sf::Vector3f> dots, std::vector<std::vector<int>> faces):
dots_(dots),
faces_(faces)
{
}
