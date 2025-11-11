#pragma once
#include <SFML/Graphics.hpp>

class RenderEngine{
public:
    RenderEngine() = default;
    // sf::RenderWindow& window;
    //фреймбуфер;
    
    // void renderMesh(const Mesh& mesh, const Transform& transform, 
    //                 const Camera& camera, const Material& material);
    // void applyLighting(const Scene& scene);
    // void postProcess();

    // void render(const Scene& scene);
        
};








// int MAP_SC = 1000;
// const float FOV = 90;
// constexpr float PI = 3.14159;
// const std::string mapFilePath = "res/clif_and_cave.obj";

// constexpr inline float toRad(float num)
// {
//     return num * PI / 180;
// }

// const float rFOV = toRad(FOV);




// sf::RenderWindow window(sf::VideoMode({SCR_X, SCR_Y}), "Island");
//     window.setFramerateLimit(60);

    
    
//     //////////////////////////// Проецирование на двумерную плоскость/////////////////////////


//     float focalLength = SCR_X / (2.0f * tan(rFOV / 2.0f));  
//     std::println("focal length: {}", focalLength);
//     float moveSpeed = 10;

    

//     // Для FPS
//     sf::Clock fpsClock;
//     int fpsFrameCount = 0;
//     float fps = 0.0f;
//     float fpsUpdateTime = 0.0f;

//     sf::Clock dtClock;
//     float dt;
//     while (window.isOpen())
//     {
//         dt = dtClock.restart().asSeconds();
//         window.clear();

//         fpsFrameCount++;
//         fpsUpdateTime += fpsClock.restart().asSeconds();
//         if (fpsUpdateTime >= 1.0f) {
//             fps = fpsFrameCount / fpsUpdateTime;
//             fpsFrameCount = 0;
//             fpsUpdateTime = 0.0f;
//         }

//         std::vector<sf::RectangleShape> pointShapes;
//         for (const auto& dot : dots) {
//             sf::Vector3f fromEyeToDot = dot - camera_position;
            
//             // Сначала поворот вокруг Y (yaw)
//             float cosYaw = cos(camera_yaw);
//             float sinYaw = sin(camera_yaw);
//             float rotatedX = fromEyeToDot.x * cosYaw - fromEyeToDot.z * sinYaw;
//             float rotatedZ = fromEyeToDot.x * sinYaw + fromEyeToDot.z * cosYaw;
            
//             // Затем поворот вокруг X (pitch)
//             float cosPitch = cos(camera_pitch);
//             float sinPitch = sin(camera_pitch);
//             float rotatedY = fromEyeToDot.y * cosPitch - rotatedZ * sinPitch;
//             rotatedZ = fromEyeToDot.y * sinPitch + rotatedZ * cosPitch;
            
//             // Теперь rotatedX, rotatedY, rotatedZ - это координаты в системе камеры
            
//             if(rotatedZ <= 0)
//                 continue;

//             float pointX = (rotatedX * focalLength) / rotatedZ;
//             float pointY = (rotatedY * focalLength) / rotatedZ;

//             sf::RectangleShape point({1, 1}); 
//             point.setPosition({SCR_X / 2 + pointX, SCR_Y / 2 - pointY});
//             point.setFillColor(sf::Color::Yellow); 
//             pointShapes.push_back(point);
//         }
        
        
        
//         while (const std::optional event = window.pollEvent())
//         {
//             if (event->is<sf::Event::Closed>())
//             {
//                 window.close();
//             }
//             if (event->is<sf::Event::KeyPressed>()) 
//             {
//                 const sf::Vector3f& c = camera_position;
//                 std::cout << "Camera position: "<< c.x << " " << c.y << " " << c.z << std::endl;
        
//             }
//         }

//         // Управление камерой
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
//             camera_position.z += moveSpeed;
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
//             camera_position.z -= moveSpeed;
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
//             camera_position.x -= moveSpeed;
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
//             camera_position.x += moveSpeed;
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
//             camera_position.y -= moveSpeed;
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
//             camera_position.y += moveSpeed;
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
//             camera_yaw -= 0.02f;
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
//             camera_yaw += 0.02f;
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
//             camera_pitch += 0.02f;
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
//             camera_pitch -= 0.02f;
//         }
                
//         // Ускорение с Shift
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
//             moveSpeed = 5000.0f * dt;
//         }
//         else 
//         {
//             moveSpeed = 1000.0f * dt;
//         }

//         for (const auto& point : pointShapes) {
//             window.draw(point);
//         }
       
        
      

//         window.display();
//     }
