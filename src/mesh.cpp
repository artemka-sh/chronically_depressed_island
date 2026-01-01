#include "mesh.hpp"

Mesh::Mesh(std::vector<sf::Vector3f> dots, std::vector<std::vector<int>> faces,
         sf::Vector3f position, bool is_static, sf::Vector3f scale, uint8_t lightness, Dimens dimen):
dots_(std::move(dots)),
faces_(std::move(faces)),
position_(position),
isStatic_(is_static),
scale_(scale),
lightness_(lightness),
dimen_(dimen)

{
}
