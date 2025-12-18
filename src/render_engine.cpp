 #include "render_engine.hpp"

int MAP_SC = 1000;
const float FOV = 90;
constexpr float PI = 3.14159;
const std::string mapFilePath = "res/clif_and_cave.obj";


constexpr inline float toRad(float num)
{
    return num * PI / 180;
}

const float rFOV = toRad(FOV);


 RenderEngine::RenderEngine(sf::RenderWindow& window, Object_loader& loader):
 window_(window), loader_(loader)
 {
    
    SCR_X = window_.getSize().x;
    SCR_Y = window_.getSize().y;
    
    focalLength = SCR_X / (2.0f * tan(rFOV / 2.0f));    //возможность создания recalculate функции для изменения поля зрения в реальном времени
 }

 void RenderEngine::render()
 {
        
    
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

    std::vector<sf::RectangleShape> pointShapes;
    for (const auto& dot : dots) {
        sf::Vector3f fromEyeToDot = dot - camera_position;
        
        // Сначала поворот вокруг Y (yaw)
        float cosYaw = cos(camera_yaw);
        float sinYaw = sin(camera_yaw);
        float rotatedX = fromEyeToDot.x * cosYaw - fromEyeToDot.z * sinYaw;
        float rotatedZ = fromEyeToDot.x * sinYaw + fromEyeToDot.z * cosYaw;
        
        // Затем поворот вокруг X (pitch)
        float cosPitch = cos(camera_pitch);
        float sinPitch = sin(camera_pitch);
        float rotatedY = fromEyeToDot.y * cosPitch - rotatedZ * sinPitch;
        rotatedZ = fromEyeToDot.y * sinPitch + rotatedZ * cosPitch;
        
        // Теперь rotatedX, rotatedY, rotatedZ - это координаты в системе камеры
        
        if(rotatedZ <= 0)
            continue;

        float pointX = (rotatedX * focalLength) / rotatedZ;
        float pointY = (rotatedY * focalLength) / rotatedZ;

        sf::RectangleShape point({1, 1}); 
        point.setPosition({SCR_X / 2 + pointX, SCR_Y / 2 - pointY});
        point.setFillColor(sf::Color::Yellow); 
        pointShapes.push_back(point);
    }
    
    for (const auto& point : pointShapes) {
        window_.draw(point);
    }

    window_.display();
               
 }