 #include "render_engine.hpp"

int MAP_SC = 1000;
constexpr float PI = 3.14159;
const std::string mapFilePath = "res/clif_and_cave.obj";



RenderEngine::RenderEngine(sf::RenderWindow& window, Object_loader& loader, Cameraman& cameraman):
window_(window), loader_(loader), cameraman_(cameraman)
{

    SCR_X = window_.getSize().x;
    SCR_Y = window_.getSize().y;

}

void RenderEngine::render()
{


    const float rFOV = cameraman_.FOV_ * PI / 180; //радиан
    const float focalLength = SCR_X / (2.0f * tan(rFOV / 2.0f));

    window_.clear();

    auto& meshes = loader_.getMeshVec();
    for (auto &currentMesh: meshes)
    {
        const std::vector<sf::Vector3f>& dots = currentMesh->dots_;
        std::map<int, sf::Vector2f> pointsOnScreen;
        int vertexIndex = 0;
        for (const auto& dot : dots)
        {
            vertexIndex++;

            sf::Vector3f fromEyeToDot = dot - cameraman_.position_;

            float cosYaw = cos(cameraman_.yaw_);
            float sinYaw = sin(cameraman_.yaw_);
            float rotatedX = fromEyeToDot.x * cosYaw - fromEyeToDot.z * sinYaw;
            float rotatedZ = fromEyeToDot.x * sinYaw + fromEyeToDot.z * cosYaw;

            float cosPitch = cos(cameraman_.pitch_);
            float sinPitch = sin(cameraman_.pitch_);
            float rotatedY = fromEyeToDot.y * cosPitch - rotatedZ * sinPitch;
            rotatedZ = fromEyeToDot.y * sinPitch + rotatedZ * cosPitch;

            // Теперь rotatedX, rotatedY, rotatedZ - это координаты в системе камеры

            if(rotatedZ <= 0)
                continue;

            float pointX = (rotatedX * focalLength) / rotatedZ;
            float pointY = (rotatedY * focalLength) / rotatedZ;

            pointsOnScreen[vertexIndex] = sf::Vector2f(SCR_X / 2 + pointX, SCR_Y / 2 - pointY);
        }

        //здесь происходит реализцаия сортировки граней по удалённости
        auto isVisible = [&](const std::vector<int>& face) {
            return pointsOnScreen.contains(face[0]) &&
                   pointsOnScreen.contains(face[1]) &&
                   pointsOnScreen.contains(face[2]);
        };

        auto visibleFaces = currentMesh->faces_
                          | std::views::filter(isVisible)
                          | std::ranges::to<std::vector>();

        std::ranges::sort(visibleFaces,
            std::greater<>{},
        [&](const std::vector<int>& face)
        {
            sf::Vector3f middleDotOfFace(0, 0, 0);
            for (int index : face)
            {
                middleDotOfFace += dots[index - 1];
            }
            middleDotOfFace /= static_cast<float>(face.size());
            sf::Vector3f diff = middleDotOfFace - cameraman_.position_;
            return diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
        }
        );
        /////////////////////////////////////////

        std::vector<sf::VertexArray> poligonVector;
        for (const auto& triangle : visibleFaces)
        {

            sf::VertexArray poligon(sf::PrimitiveType::Triangles, 3); //
            poligon[0] = sf::Vertex(pointsOnScreen[triangle[0]], sf::Color::Yellow);
            poligon[1] = sf::Vertex(pointsOnScreen[triangle[1]], sf::Color::Magenta);
            poligon[2] = sf::Vertex(pointsOnScreen[triangle[2]], sf::Color::Green);

            poligonVector.push_back(poligon);
        }

        //std::random_shuffle(poligonVector.begin(), poligonVector.end());

        for (const auto& poligon: poligonVector)
        {
            window_.draw(poligon);
        }


    }

    window_.display();

}