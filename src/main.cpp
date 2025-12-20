#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <print>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>


#include "../include/application.hpp"

int SCR_X = 1280, SCR_Y = 720;

int main()
{
    
    Application app(SCR_X, SCR_Y, "iceland");
    
    return app();
}
