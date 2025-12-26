 #include "render_engine.hpp"

sf::Vector3f crossProduct(const sf::Vector3f& a, const sf::Vector3f& b);

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
        // std::unordered_map<int, sf::Vector2f> pointsOnScreen;
        std::vector<sf::Vector2f> pointsOnScreen(dots.size() + 1, {NAN, NAN});
        int vertexIndex = 0;


        float cosYaw = cos(cameraman_.yaw_);
        float sinYaw = sin(cameraman_.yaw_);

        float cosPitch = cos(cameraman_.pitch_);
        float sinPitch = sin(cameraman_.pitch_);


        for (const auto& dot : dots)
        {
            vertexIndex++;

            sf::Vector3f fromEyeToDot = dot - cameraman_.position_;

            float rotatedX = fromEyeToDot.x * cosYaw - fromEyeToDot.z * sinYaw;
            float rotatedZ = fromEyeToDot.x * sinYaw + fromEyeToDot.z * cosYaw;

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
            return !std::isnan(pointsOnScreen[face[0]].x) &&
                   !std::isnan(pointsOnScreen[face[1]].x) &&
                   !std::isnan(pointsOnScreen[face[2]].x);
        };
        visibleFaces_ = currentMesh->faces_
                          | std::views::filter(isVisible)
                          | std::ranges::to<std::vector>();

        std::ranges::sort(visibleFaces_,
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
        for (const auto& triangle : visibleFaces_)
        {
            const sf::Vector3f& A = dots[triangle[0] - 1];
            const sf::Vector3f& B = dots[triangle[1] - 1];
            const sf::Vector3f& C = dots[triangle[2] - 1];
            const auto AB = A - B;
            const auto AC = A - C;

            sf::Vector3f normalVector = crossProduct(AB, AC).normalized();

            float scalarProduct = normalVector.dot(sunLightDirection);

            int br = scalarProduct * 128;
            br = std::clamp(br, 0, 255);
            sf::Color pColor(br, br, br);

            sf::VertexArray poligon(sf::PrimitiveType::Triangles, 3); //
            poligon[0] = sf::Vertex(pointsOnScreen[triangle[0]], pColor);
            poligon[1] = sf::Vertex(pointsOnScreen[triangle[1]], pColor);
            poligon[2] = sf::Vertex(pointsOnScreen[triangle[2]], pColor);

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

sf::Vector3f crossProduct(const sf::Vector3f& a, const sf::Vector3f& b) {
    return sf::Vector3f(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}
