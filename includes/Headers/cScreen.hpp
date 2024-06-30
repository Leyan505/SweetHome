#ifndef CSCREEN_HEADER
#define CSCREEN_HEADER
#include<SFML/Graphics.hpp>

class cScreen
{
    public:
        virtual int Run (sf::RenderWindow &App) = 0;
};

#endif