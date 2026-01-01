#include "object_loader.hpp"

Object_loader::Object_loader(ResourceManager& resource_manager):
resourceManager_(resource_manager)
{
    auto cliff = std::make_shared<Mesh>();
    cliff->dimen_ = Mesh::Dimens::_3d;
    cliff->position_ = sf::Vector3f(0, 0, 0);
    cliff->isStatic_ = true;
    cliff->lightness_ = false;
    cliff->scale_ = sf::Vector3f(100.0f, 100.0f, 100.0f);
    cliff->one_sound_ = nullptr; //тоже можно чёнить для вайба также для звуков нужно сделать расстояние
    cliff->texture_ = nullptr;
    load_3d_data(cliff, "res/clif_and_cave.obj");
    objects.push_back(cliff);

    auto fire = std::make_shared<Mesh>();
    fire->dimen_ = Mesh::Dimens::_2d;
    fire->position_ = sf::Vector3f(10, 10, 10);
    fire->isStatic_ = true;
    fire->lightness_ = true;
    fire->scale_ = sf::Vector3f(300.0f, 400.0f, 0.0f);
    fire->one_sound_ = resourceManager_.get<sf::SoundBuffer>("res/campfire.wav");
    fire->texture_ = resourceManager_.get<sf::Texture>("res/campfire.png");
    objects.push_back(fire);
}

Object_loader::~Object_loader()
{

}

std::shared_ptr<Mesh> Object_loader::load_mesh()
{
    auto mesh = std::make_shared<Mesh>();
    return mesh;
}

std::shared_ptr<Mesh> Object_loader::load_mesh(std::string model_path)
{
    auto mesh = std::make_shared<Mesh>();
    objects.push_back(mesh);
    return mesh;

}

std::vector<std::shared_ptr<Mesh>>& Object_loader::getMeshVec()
{
    return objects;
}

void Object_loader::load_3d_data(std::shared_ptr<Mesh> mesh, const std::string& path)
{
    std::ifstream mapfile(path);
    ////////////////////////////////// Парсинг точкек /////////////////////////////////////


    if (!mapfile) {
        std::println("Ошибка открытия файла, не получилось создать поток чтения файла");
        throw std::runtime_error("object_loader ожидал что файл будет а его нет");
    }

    std::vector<sf::Vector3f>& dots = mesh->dots_;
    std::vector<std::vector<int>>& faces = mesh->faces_;
    for(std::string line; std::getline(mapfile, line); )
    {
        if(line.starts_with("v "))
        {

            std::istringstream iss(line.substr(2));
            float x, y, z;

            if (iss >> x >> y >> z) {
                dots.emplace_back(
                    x * mesh->scale_.x + mesh->position_.x,
                    y * mesh->scale_.y + mesh->position_.y,
                    z * mesh->scale_.z + mesh->position_.z);
            }
            else
            {
                throw std::runtime_error("wrong with parsing");
            }
        }

        if(line.starts_with("f "))
        {
            std::istringstream iss(line.substr(2));

            std::vector<int> vertices;
            std::string token;
            while (iss >> token) {
                vertices.emplace_back
                (
                    std::stoi(token.substr(0, token.find('/'))) - 1
                );
            }
            faces.push_back(vertices);
        }

    }

    mapfile.close();
    mapfile.clear();

    // for(auto x: dots)
    // {
    //     std::cout << x.x <<  " " << x.y << " " << x.z << std::endl;
    // }

    std::println("dots size: {}, for {}",  dots.size(), path);
    std::println("faces count: {}, for {}", faces.size(), path);

    //объект сформирован
}