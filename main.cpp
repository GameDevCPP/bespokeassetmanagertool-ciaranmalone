#include <SFML/Graphics.hpp>
#include <iostream>
#include "TileMap.cpp"
#include "json.hpp"
#include <fstream>

namespace ns{

    struct MapForJsonStruct{
        std::vector<int> map;
        std::string textureForMap;
        std::vector<int> tileSize, mapSize; 

        const int* get_level()
        {
            //since vector must store data contiguously, the following works for array
            int* a = &map[0];
            return a;
        }
    };

    void to_json(nlohmann::json& j, const MapForJsonStruct& ms){
        j = nlohmann::json{
            {"map",ms.map},
            {"tilesize",ms.tileSize},
            {"mapsize",ms.mapSize},
            {"maptexture",ms.textureForMap}
        };
    }

    void from_json(const nlohmann::json& j, MapForJsonStruct& ms){
        j.at("map").get_to(ms.map);
        j.at("tilesize").get_to(ms.tileSize);
        j.at("mapsize").get_to(ms.mapSize);
        j.at("maptexture").get_to(ms.textureForMap);
    }
}

void updateJson(ns::MapForJsonStruct ms) {

    using nlohmann::json;

    json j2 = ms;

    std::ofstream outJson("map.json");
    outJson << j2 << std::endl;

}

int main()
{
    int TileType = 1;

    using nlohmann::json;

    sf::RenderWindow window(sf::VideoMode(800, 600), "TileMap Maker V0.1", sf::Style::Close |  sf::Style::Resize);

    std::ifstream file ("map.json");

    json j;
    file >> j;
    auto ms = j.get<ns::MapForJsonStruct>();

    window.setVerticalSyncEnabled(true);

    TileMap map;

    map.load(ms.textureForMap, sf::Vector2u(ms.tileSize[0], ms.tileSize[1]), ms.get_level(), ms.mapSize[0], ms.mapSize[1]);

    while (window.isOpen())
    {
        //window pull event
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {

            case sf::Event::Closed:
                updateJson(ms);
                window.close();
                break;
            
            case sf::Event::Resized:
                // std::cout << " Window Width: " << event.size.width << "Window Height" << event.size.height << std::endl;
                printf("Window Width: %i Window Height %i \n", event.size.width, event.size.height);
                break;

            case sf::Event::TextEntered:
                    printf("%c", event.text.unicode);
                break;

            default:
                break;
            }
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            int col = floor(mousePos.x/ms.tileSize[0] ) + 1;          
            int row = floor(mousePos.y/ms.tileSize[1] ) + 1; 

            if (col <= ms.mapSize[0] && row <= ms.mapSize[1]) {
                
                ms.map[((row-1) * ms.mapSize[0]) + (col-1)] = TileType;
                map.load(ms.textureForMap, sf::Vector2u(ms.tileSize[0], ms.tileSize[1]), ms.get_level(), ms.mapSize[0], ms.mapSize[1]);
                printf("x: %i y %i \n", col, row);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            TileType = 0;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            TileType = 1;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
            TileType = 2;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
            TileType = 3;
        

        window.clear();
        window.draw(map);
        window.display();
    }

    return 0;
}

