// std::ifstream mapfile(mapFilePath);
//     ////////////////////////////////// Парсинг точкек /////////////////////////////////////     //дважды читаем файл

 

//     if (!mapfile) {
//         std::cerr << "Ошибка открытия файла!" << std::endl;
//         return 1;
//     }

//     std::vector<sf::Vector3f> dots; 
//     for(std::string line; std::getline(mapfile, line); )
//     {
//         if(!line.starts_with("v "))
//         continue;

//         std::istringstream iss(line.substr(2)); 
//         float x, y, z;

//         if (iss >> x >> y >> z) {
//             x*=MAP_SC,
//             y*=MAP_SC,
//             z*=MAP_SC;
//             dots.emplace_back(x, y, z); 
//         }
//         else
//         {
//             std::cerr << "wrong with parsing" << std::endl;
//         }
//     }
    
//     mapfile.close();
//     mapfile.clear();
//     mapfile.open(mapFilePath);

//     // for(auto x: dots)
//     // {
//     //     std::cout << x.x <<  " " << x.y << " " << x.z << std::endl;
//     // }
//     std::cerr << "dots size: " <<  dots.size() << std::endl;

//     ///////////////////////////// Парсинг Граней /////////////////////////////////////////



//     if (!mapfile) {
//         std::cerr << "Ошибка открытия файла!" << std::endl;
//         return 1;
//     }


//     std::vector<std::vector<int>> faces; 
//     for(std::string line; std::getline(mapfile, line); )
//     {

//         if(!line.starts_with("f "))
//         continue;
//         std::istringstream iss(line.substr(2)); 
        
//         std::vector<int> vertices;
//         std::string token;
//         std::cout << "face: ";
//         while (iss >> token) {
//             int& num = 
//             vertices.emplace_back
//             (
//                 std::stoi(token.substr(0, token.find('/')))
//             );;
//             std::cout << num << " ";
//         }
//         faces.push_back(vertices);
//         std::cout << "\n";
       
//     }
//     std::println("faces count: {}", faces.size());

    

//     sf::Vector3f camera_position{0.0f, 0.0f, 0.0f};
//     float camera_yaw = 0.0f;   
//     float camera_pitch = 0.0f;
