#include "object_loader.hpp"

Object_loader::Object_loader(/* args */)
{
    this->load_mesh("res/clif_and_cave.obj");
}

Object_loader::~Object_loader()
{
}

std::shared_ptr<Mesh> Object_loader::load_mesh(std::string model_path)
{
    auto mesh = std::make_shared<Mesh>();

    std::ifstream mapfile(model_path);
    ////////////////////////////////// Парсинг точкек /////////////////////////////////////    


    if (!mapfile) {
        std::println("Ошибка открытия файла!");

        throw std::runtime_error("ошибка открытия файла( object_loader ожидал что файл будет а его нет");
        return nullptr;
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
                //если что-то тне так, тут домножалось на 100 чтоб было не всё маленькое
                dots.emplace_back(x*100, y*100, z*100); 
            }
            else
            {
                std::println("wrong with parsing");
            }
        }

        if(line.starts_with("f "))
        {
            std::istringstream iss(line.substr(2)); 

            std::vector<int> vertices;
            std::string token;
            std::cout << "face: ";
            while (iss >> token) {
                int& num = 
                vertices.emplace_back
                (
                    std::stoi(token.substr(0, token.find('/')))
                );;
                std::cout << num << " ";
            }
            faces.push_back(vertices);
            std::cout << "\n";
        }
     
    }

    mapfile.close();
    mapfile.clear();

    // for(auto x: dots)
    // {
    //     std::cout << x.x <<  " " << x.y << " " << x.z << std::endl;
    // }

    std::println("dots size: {}",  dots.size());
    std::println("faces count: {}", faces.size());

    objects.push_back(mesh);
    return mesh;

}

std::vector<std::shared_ptr<Mesh>>& Object_loader::getMeshVec()
{
    return objects;
}