#include <Headers/Menu.h>
#include <Headers/filesystem.h>
#include <iostream>


Menu::Menu(float width, float height)
{
    if(!font.loadFromFile("HalloweenPartySt-14pB.ttf"));
    {
        std::cout<<"ERROR LOADING FONT"<<std::endl;
    }

    option[0].setFont(font);
    option[0].setFillColor(sf::Color::White);
    option[0].setString("Living Room");
    option[0].setPosition(sf::Vector2f(width/2, height/(MAX_NUMBER_OPTIONS+1)*1));

    option[1].setFont(font);
    option[1].setFillColor(sf::Color::White);
    option[1].setString("Kitchen");
    option[1].setPosition(sf::Vector2f(width/2, height/(MAX_NUMBER_OPTIONS+1)*2));

    option[2].setFont(font);
    option[2].setFillColor(sf::Color::White);
    option[2].setString("Bedroom");
    option[2].setPosition(sf::Vector2f(width/2, height/(MAX_NUMBER_OPTIONS+1)*3));

    option[3].setFont(font);
    option[3].setFillColor(sf::Color::White);
    option[3].setString("Corridor");
    option[3].setPosition(sf::Vector2f(width/2, height/(MAX_NUMBER_OPTIONS+1)*4));
}

void Menu::draw(sf::RenderWindow &window)
{
    for(int i = 0; i < MAX_NUMBER_OPTIONS; i++)
    {
        window.draw(option[i]);
    }
}