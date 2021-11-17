#include <iostream>
#include "TileMap.cpp"
#include "json.hpp"
#include <fstream>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

int TileType = 1;

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

void updateText(int num) {
    TileType = num;
}

int main()
{

    using nlohmann::json;

    sf::RenderWindow window(sf::VideoMode(800, 600), "TileMap Maker V0.1", sf::Style::Close |  sf::Style::Resize);
    tgui::Gui gui{window};

    auto child = tgui::ChildWindow::create();
    // child->setRenderer(theme.getRenderer("ChildWindow"));
    child->setClientSize({120, 250});
    child->setPosition(420, 80);
    child->setTitle("Child window");
    gui.add(child);

    auto button = tgui::Button::create();
    button->setPosition(0, 10);
    button->setText("TILE 1");
    button->setSize(100, 30);
    button->onPress(&updateText, 0);
    child->add(button);

    auto button1 = tgui::Button::create();
    button1->setPosition(0, 40);
    button1->setText("TILE 2");
    button1->setSize(100, 30);
    button1->onPress(&updateText, 1);
    child->add(button1);

    auto button2 = tgui::Button::create();
    button2->setPosition(0, 70);
    button2->setText("TILE 3");
    button2->setSize(100, 30);
    button2->onPress(&updateText, 2);
    child->add(button2);

    auto button3 = tgui::Button::create();
    button3->setPosition(0, 100);
    button3->setText("TILE 4");
    button3->setSize(100, 30);
    button3->onPress(&updateText, 3);
    child->add(button3);

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
            gui.handleEvent(event);

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
        gui.draw();
        window.display();
    }

    return 0;
}
