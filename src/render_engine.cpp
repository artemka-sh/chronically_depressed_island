 #include "render_engine.hpp"


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
    float distance = sqrt(toMesh.x * toMesh.x + toMesh.y * toMesh.y + toMesh.z * toMesh.z);
    float meshHeight = mesh.scale_.y * focalLength / distance;
    float meshWidth = mesh.scale_.x * focalLength / distance;



    std::vector<sf::Vector2f> pointsOnScreen(4);
    pointsOnScreen[0] = centerPoint + sf::Vector2f(-meshWidth/2, -meshHeight);
    pointsOnScreen[1] = centerPoint + sf::Vector2f(meshWidth/2, -meshHeight);
    pointsOnScreen[2] = centerPoint + sf::Vector2f(meshWidth/2, 0);
    pointsOnScreen[3] = centerPoint + sf::Vector2f(-meshWidth/2, 0);

    sf::Vector2f texSize = static_cast<sf::Vector2f>(mesh.texture_->getSize());
    sf::VertexArray vertices(sf::PrimitiveType::Triangles, 6);
    vertices[0].position = pointsOnScreen[0]; vertices[0].texCoords = sf::Vector2f(0.f, 0.f);
    vertices[1].position = pointsOnScreen[1]; vertices[1].texCoords = sf::Vector2f(texSize.x, 0.f);
    vertices[2].position = pointsOnScreen[3]; vertices[2].texCoords = sf::Vector2f(0.f, texSize.y);
    vertices[3].position = pointsOnScreen[1]; vertices[3].texCoords = sf::Vector2f(texSize.x, 0.f);
    vertices[4].position = pointsOnScreen[2]; vertices[4].texCoords = sf::Vector2f(texSize.x, texSize.y);
    vertices[5].position = pointsOnScreen[3]; vertices[5].texCoords = sf::Vector2f(0.f, texSize.y);


    window_.draw(vertices, mesh.texture_.get());

}

void RenderEngine::render3DMesh(const Mesh& mesh)
{
    const Mesh *currentMesh = &mesh; //это просто чтоб не менять имя и споосб обращения

    const auto& dots = currentMesh->dots_;
    std::vector<sf::Vector2f> pointsOnScreen = calculateDots(dots);


    //здесь происходит реализцаия сортировки граней по удалённости
    auto isVisible = [&](const std::vector<int>& face) {
        return !std::isnan(pointsOnScreen[face[0]].x) ||
               !std::isnan(pointsOnScreen[face[1]].x) ||
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
            middleDotOfFace += dots[index];
        }
        middleDotOfFace /= static_cast<float>(face.size());
        sf::Vector3f diff = middleDotOfFace - cameraman_.position_;
        return diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
    }
    );
    /////////////////////////////////////////



    // std::vector<sf::VertexArray> poligonVector;
    sf::VertexArray poligonVector(sf::PrimitiveType::Triangles, visibleFaces_.size() * 3);
    for (const auto& triangle : visibleFaces_)
    {
        const sf::Vector3f& A = dots[triangle[0]];
        const sf::Vector3f& B = dots[triangle[1]];
        const sf::Vector3f& C = dots[triangle[2]];
        const auto AB = A - B;
        const auto AC = A - C;

        sf::Vector3f normalVector = crossProduct(AB, AC).normalized();

        float scalarProduct = normalVector.dot(sunLightDirection);

        int br = scalarProduct * 128;
        br = std::clamp(br, 0, 255);
        sf::Color pColor(br, br, br);

        sf::VertexArray poligon(sf::PrimitiveType::Triangles, 3); //
        poligonVector.append(sf::Vertex(pointsOnScreen[triangle[0]], pColor));
        poligonVector.append(sf::Vertex(pointsOnScreen[triangle[1]], pColor));
        poligonVector.append(sf::Vertex(pointsOnScreen[triangle[2]], pColor));
    }

    //std::random_shuffle(poligonVector.begin(), poligonVector.end());
    // for (const auto& poligon: poligonVector)

    window_.draw(poligonVector);


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