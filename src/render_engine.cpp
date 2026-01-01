 #include "render_engine.hpp"


sf::Vector3f crossProduct(const sf::Vector3f& a, const sf::Vector3f& b);

int MAP_SC = 1000;
constexpr float PI = 3.14159;


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
    std::vector<sf::Vector3f> dots(4);
    // dots.resize(4);

    float halfWidth = mesh.scale_.x / 2.0f;
    float halfHeight = mesh.scale_.y / 2.0f;

    float cosY = cos(cameraman_.yaw_);
    float sinY = sin(cameraman_.yaw_);


    sf::Vector3f camera_right(cosY, 0.0f, sinY);
    sf::Vector3f up = sf::Vector3f(0.0f, 0.1f, 0.0f) * halfHeight;

    dots[0] = mesh.position_ - camera_right + up; //A
    dots[1] = mesh.position_ + camera_right + up; //B
    dots[2] = mesh.position_ + camera_right - up; //C
    dots[3] = mesh.position_ - camera_right - up; //D

    std::vector<sf::Vector2f> pointsOnScreen = calculateDots(dots);

    // if (std::isnan(pointsOnScreen[1].x) ||
    //     std::isnan(pointsOnScreen[2].x) ||
    //     std::isnan(pointsOnScreen[3].x) ||
    //     std::isnan(pointsOnScreen[4].x))
    // {
    //     return;
    // }


    sf::VertexArray vertices(sf::PrimitiveType::Triangles, 6);

    sf::Vector2f texSize = static_cast<sf::Vector2f>(mesh.texture_->getSize());


    sf::Vector2f p0(100.f, 100.f); // Верх-Лево
    sf::Vector2f p1(400.f, 50.f);  // Верх-Право (чуть выше)
    sf::Vector2f p2(450.f, 350.f); // Низ-Право  (чуть правее)
    sf::Vector2f p3(50.f,  300.f); // Низ-Лево   (чуть левее)


    // Треугольник 1 (p0, p1, p3)
    vertices[0].position = p0;
    vertices[0].texCoords = sf::Vector2f(0.f, 0.f);

    vertices[1].position = p1;
    vertices[1].texCoords = sf::Vector2f(texSize.x, 0.f);

    vertices[2].position = p3;
    vertices[2].texCoords = sf::Vector2f(0.f, texSize.y);

    vertices[3].position = p1;
    vertices[3].texCoords = sf::Vector2f(texSize.x, 0.f);

    vertices[4].position = p2;
    vertices[4].texCoords = sf::Vector2f(texSize.x, texSize.y);

    vertices[5].position = p3;
    vertices[5].texCoords = sf::Vector2f(0.f, texSize.y);

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

    std::vector<sf::VertexArray> poligonVector;
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