#include "json.hpp"
#include <SFML/Graphics.hpp>
#include "TileMap.cpp"
#include <iostream>
#include <vector>
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


void DrawMap(ns::MapForJsonStruct ms);

int main()
{
    using nlohmann::json;
    std::ofstream outJson("map.json");
  
    // create the window
    sf::RenderWindow window(sf::VideoMode(512, 256), "Tilemap");
    window.setVerticalSyncEnabled(true);
    
    sf::Vector2u widthAndHeight(16, 8), tileSize(32,32);
    
    
    //output map as json object
    json jsonMap={
        {"map",
            {0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
            0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
            0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0 , 0,
            0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
            2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
            0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1}},
        {"tilesize",{tileSize.x,tileSize.y}},
        {"mapsize",{widthAndHeight.x,widthAndHeight.y}},
        {"maptexture","tileset.png"}
    };
    
    
    ns::MapForJsonStruct map2;
    json j = map2;
    
    
    //write json object:
    outJson << jsonMap<< std::endl;
    
    
    std::ifstream i("map.json");
    i >> j;
    std::cout << j;
    auto p2 = j.get<ns::MapForJsonStruct>();
    std::cout <<std::endl<<" p2 =";
    for (auto i : p2.map)
        std::cout << i << ' ';
    std::cout <<  ", mapsize = [" << p2.mapSize[0] << ","<< p2.mapSize[1]<<"]"<<std::endl;
    // create the tilemap from the level definition
    TileMap map;
   
    //read in our data-driven-game -- well just a map - but it's a start!
    DrawMap(p2);

    return EXIT_SUCCESS;
   
};

void DrawMap(ns::MapForJsonStruct ms)
{

    sf::RenderWindow window(sf::VideoMode(512, 256), "Tilemap");
    window.setVerticalSyncEnabled(true);

    // define the level with an array of tile indices
    auto level = ms.get_level();//I know return type is const int*.

    // create the tilemap from the level definition
    TileMap map;
    
    if (map.load(ms.textureForMap, sf::Vector2u(ms.tileSize[0], ms.tileSize[1]), ms.get_level(), ms.mapSize[0], ms.mapSize[1])) 
    {

        // run the main loop
        while (window.isOpen())
        {
            // handle events
            sf::Event event;
            while (window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                    window.close();
            }

            // draw the map
            window.clear();
            window.draw(map);
            window.display();
        }
    }
}