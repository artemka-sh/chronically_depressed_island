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
    sf::Vector3f& camera_position  = cameraman_.position_;
    const float& camera_yaw = cameraman_.yaw_;
    const float& camera_pitch = cameraman_.pitch_;
    const float& FOV = cameraman_.FOV_;
    const float rFOV = FOV * PI / 180;
    const float focalLength = SCR_X / (2.0f * tan(rFOV / 2.0f));    //оверхед повсюду(



    auto& meshes = loader_.getMeshVec();
    std::shared_ptr<Mesh> currentMesh = meshes[0];
    const std::vector<sf::Vector3f>& dots = currentMesh->dots_;

    window_.clear();

    fpsFrameCount++;
    fpsUpdateTime += fpsClock.restart().asSeconds();
    if (fpsUpdateTime >= 1.0f) {
        fps = fpsFrameCount / fpsUpdateTime;
        fpsFrameCount = 0;
        fpsUpdateTime = 0.0f;
    }


    std::map<int, sf::Vector2f> pointsOnScreen;
    int vertexIndex = 0;
     for (const auto& dot : dots)
     {
         vertexIndex++;

        sf::Vector3f fromEyeToDot = dot - camera_position;

        float cosYaw = cos(camera_yaw);
        float sinYaw = sin(camera_yaw);
        float rotatedX = fromEyeToDot.x * cosYaw - fromEyeToDot.z * sinYaw;
        float rotatedZ = fromEyeToDot.x * sinYaw + fromEyeToDot.z * cosYaw;

        float cosPitch = cos(camera_pitch);
        float sinPitch = sin(camera_pitch);
        float rotatedY = fromEyeToDot.y * cosPitch - rotatedZ * sinPitch;
        rotatedZ = fromEyeToDot.y * sinPitch + rotatedZ * cosPitch;

        // Теперь rotatedX, rotatedY, rotatedZ - это координаты в системе камеры

        if(rotatedZ <= 0)
            continue;

        float pointX = (rotatedX * focalLength) / rotatedZ;
        float pointY = (rotatedY * focalLength) / rotatedZ;

         pointsOnScreen[vertexIndex] = sf::Vector2f(SCR_X / 2 + pointX, SCR_Y / 2 - pointY);
    }


    for (const auto& triangle : currentMesh->faces_)
    {
        if (pointsOnScreen.count(triangle[0]) == 0 ||
            pointsOnScreen.count(triangle[1]) == 0 ||
            pointsOnScreen.count(triangle[2]) == 0)
        {
            continue;
        }

        sf::Vertex vertices[3];
        vertices[0] = sf::Vertex(pointsOnScreen[triangle[0]], sf::Color::Yellow);
        vertices[1] = sf::Vertex(pointsOnScreen[triangle[1]], sf::Color::Magenta);
        vertices[2] = sf::Vertex(pointsOnScreen[triangle[2]], sf::Color::Green);

        window_.draw(vertices, 3, sf::PrimitiveType::Triangles);
    }


    window_.display();

}