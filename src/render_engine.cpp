 #include "render_engine.hpp"
#include <SFML/Main.hpp>

int MAP_SC = 1000;
constexpr float PI = 3.14159;


inline float deg_to_rad(float n)
{
    return n * PI / 180;
}

sf::Vector3f crossProduct(const sf::Vector3f& a, const sf::Vector3f& b);



RenderEngine::RenderEngine(sf::RenderWindow& window, Object_loader& loader, Cameraman& cameraman):
window_(window), loader_(loader), cameraman_(cameraman)
{

    SCR_X = window_.getSize().x;
    SCR_Y = window_.getSize().y;



}

void RenderEngine::render()
{
    window_.clear();
    polygonVector.clear();

    day += 0.01f;    year += 0.01f;
    sunLightDirection = {std::cos(day), std::sin(day) + std::sin(year) * 0.4f, std::cos(year) * 0.2f};
    sunLightDirection = sunLightDirection.normalized();

    rFOV = cameraman_.FOV_ * PI / 180; //радиан
    focalLength = SCR_X / (2.0f * tan(rFOV / 2.0f));

    rendSkybox();

    auto& meshes = loader_.getMeshVec();
    for (auto &currentMesh: meshes)
    {
        if (currentMesh->dimen_ == Mesh::Dimens::_3d)
        {
            render3DMesh(*currentMesh);
        }
        else
        {
            render2DMesh(*currentMesh);
        }
    }


    std::ranges::sort(polygonVector, [](const Polygon& a, const Polygon& b) {
        return a.depth_ > b.depth_;
    });

    renderBatch();
    window_.display();
}

sf::Vector3f crossProduct(const sf::Vector3f& a, const sf::Vector3f& b) {
    return sf::Vector3f(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

void RenderEngine::render2DMesh(const Mesh& mesh)
{
    sf::Vector3f dot_position = mesh.position_;
    std::vector<sf::Vector2f> pointOnScreen = calculateDots(std::vector<sf::Vector3f>{dot_position});
    sf::Vector2f centerPoint = pointOnScreen[0];
    if (std::isnan(centerPoint.x))
    {
        return;
    }

    sf::Vector3f toMesh = cameraman_.position_ - mesh.position_;
    float depthSq = toMesh.x * toMesh.x + toMesh.y * toMesh.y + toMesh.z * toMesh.z;
    float distance = sqrt(depthSq);

    float meshHeight = mesh.scale_.y * focalLength / distance;
    float meshWidth = mesh.scale_.x * focalLength / distance;

    std::vector<sf::Vector2f> pointsOnScreen(4);
    pointsOnScreen[0] = centerPoint + sf::Vector2f(-meshWidth/2, -meshHeight);
    pointsOnScreen[1] = centerPoint + sf::Vector2f(meshWidth/2, -meshHeight);
    pointsOnScreen[2] = centerPoint + sf::Vector2f(meshWidth/2, 0);
    pointsOnScreen[3] = centerPoint + sf::Vector2f(-meshWidth/2, 0);

    sf::Vector2f texSize = static_cast<sf::Vector2f>(mesh.texture_->getSize());
    polygonVector.reserve(polygonVector.size() + 2);


    float scalarProduct = sf::Vector3f{0, 1, 0}.dot(sunLightDirection);
    int br = scalarProduct * 255;
    if (mesh.lightness_) br = std::abs(br);
    br += mesh.lightness_;
    br = std::clamp(br, 0, 255);
    sf::Color pColor(br, br, br);

    Polygon p1;
    p1.vertices[0] = {pointsOnScreen[0], pColor, {0.f, 0.f}};
    p1.vertices[1] = {pointsOnScreen[1], pColor, { texSize.x, 0.f}};
    p1.vertices[2] = {pointsOnScreen[3], pColor, {0.f,  texSize.y}};
    p1.texture_ = mesh.texture_;
    p1.depth_ = depthSq;


    Polygon p2;
    p2.vertices[0] = {pointsOnScreen[1], pColor, { texSize.x, 0.f}};
    p2.vertices[1] = {pointsOnScreen[2], pColor, { texSize.x,  texSize.y}};
    p2.vertices[2] = {pointsOnScreen[3], pColor, {0.f,  texSize.y}};
    p2.texture_ = mesh.texture_;
    p2.depth_ = depthSq;

    polygonVector.push_back(p1);
    polygonVector.push_back(p2);
}

void RenderEngine::render3DMesh(const Mesh& mesh)
{
    std::vector<sf::Vector2f> pointsOnScreen = calculateDots(mesh.dots_);

    for (const auto& triangle : mesh.faces_)
    {
        if (std::isnan(pointsOnScreen[triangle[0]].x) ||
            std::isnan(pointsOnScreen[triangle[1]].x) ||
            std::isnan(pointsOnScreen[triangle[2]].x)) {
            continue;
            }

        const sf::Vector3f& A = mesh.dots_[triangle[0]];
        const sf::Vector3f& B = mesh.dots_[triangle[1]];
        const sf::Vector3f& C = mesh.dots_[triangle[2]];
        const auto AB = A - B;
        const auto AC = A - C;

        sf::Vector3f normalVector = crossProduct(AB, AC).normalized();

        float scalarProduct = normalVector.dot(sunLightDirection);

        int br = scalarProduct * 128;
        br = std::clamp(br, 0, 255);
        sf::Color pColor(br, br, br);



        Polygon poly;
        poly.vertices[0] = sf::Vertex(pointsOnScreen[triangle[0]], pColor);
        poly.vertices[1] = sf::Vertex(pointsOnScreen[triangle[1]], pColor);
        poly.vertices[2] = sf::Vertex(pointsOnScreen[triangle[2]], pColor);

        poly.texture_ = mesh.texture_;
        sf::Vector3f center = (A + B + C) / 3.0f;
        sf::Vector3f diff = center - cameraman_.position_;
        poly.depth_ = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;

        polygonVector.emplace_back(poly);
    }


}

void RenderEngine::rendSkybox()
{

    sf::Color skyColor(30, 144, 255);      // Голубое небо (вверх)
    sf::Color horizonColor(100, 150, 255); // Средний (горизонт)
    sf::Color groundColor(128, 0, 128);    // Фиолетовая земля (вниз)


    sf::VertexArray gradient(sf::PrimitiveType::TriangleStrip, 4);
    float sx = static_cast<float>(SCR_X);
    float sy = static_cast<float>(SCR_Y);

    gradient[0] = {{0.f, 0.f}, skyColor};
    gradient[1] = {{sx, 0.f}, skyColor};
    gradient[2] = {{0.f, sy},horizonColor};
    gradient[3] = {{sx, sy}, horizonColor};

    window_.draw(gradient);
}

std::vector<sf::Vector2f> RenderEngine::calculateDots(const std::vector<sf::Vector3f>& dots)
{
    std::vector<sf::Vector2f> pointsOnScreen(dots.size(), {NAN, NAN});


    int vertexIndex = 0;


    float cosYaw = cos(cameraman_.yaw_);
    float sinYaw = sin(cameraman_.yaw_);

    float cosPitch = cos(cameraman_.pitch_);
    float sinPitch = sin(cameraman_.pitch_);


    for (const auto& dot : dots)
    {
        sf::Vector3f fromEyeToDot = dot - cameraman_.position_;

        float rotatedX = fromEyeToDot.x * cosYaw - fromEyeToDot.z * sinYaw;
        float rotatedZ = fromEyeToDot.x * sinYaw + fromEyeToDot.z * cosYaw;

        float rotatedY = fromEyeToDot.y * cosPitch - rotatedZ * sinPitch;
        rotatedZ = fromEyeToDot.y * sinPitch + rotatedZ * cosPitch;

        // Теперь rotatedX, rotatedY, rotatedZ - это координаты в системе камеры

        if(rotatedZ <= 0) //здесь надо думать, пока так, но это не туда не сюда магическое число
        { // в теори должно давать возможность смотреть на объект ближе
            vertexIndex++;
            continue;
        }


        float pointX = (rotatedX * focalLength) / rotatedZ;
        float pointY = (rotatedY * focalLength) / rotatedZ;

        pointsOnScreen[vertexIndex] = sf::Vector2f(SCR_X / 2 + pointX, SCR_Y / 2 - pointY);
        vertexIndex++;

    }

    //std::move антипаттерн, должно работать RVO (пожалуйста)
    return pointsOnScreen;
}

void RenderEngine::renderBatch()
{
    if (polygonVector.empty()) return;

    static std::vector<sf::Vertex> currentBatch;
    currentBatch.clear();
    currentBatch.reserve(polygonVector.size() * 3);

    const sf::Texture* lastTexturePtr = polygonVector[0].texture_.get();

    for (const auto& poly : polygonVector) {
        if (poly.texture_.get() != lastTexturePtr) {
            if (!currentBatch.empty()) {
                window_.draw(currentBatch.data(), currentBatch.size(), sf::PrimitiveType::Triangles, lastTexturePtr);
                currentBatch.clear();
            }
            lastTexturePtr = poly.texture_.get();
        }

        currentBatch.push_back(poly.vertices[0]);
        currentBatch.push_back(poly.vertices[1]);
        currentBatch.push_back(poly.vertices[2]);
    }

    if (!currentBatch.empty()) {
        window_.draw(currentBatch.data(), currentBatch.size(), sf::PrimitiveType::Triangles, lastTexturePtr);
    }
}