#pragma once
#include <SFML/Graphics.hpp>

#define MAX_NUMBER_OPTIONS 4

class Menu
{
    public:
        Menu(float width, float height);
        Menu();

        void draw(sf::RenderWindow &window);
        void MoveUp();
        void MoveDown();

    private:
        int selectedOpt;
        sf::Font font;
        sf::Text option[MAX_NUMBER_OPTIONS];
};

