#pragma once
#include <SFML/Graphics.hpp>

class CreditsModal {
public:
    CreditsModal(float width, float height);
    void draw(sf::RenderWindow &window);
    void show();
    void hide();
    bool isVisible() const;

private:
    sf::RectangleShape background;
    sf::Text creditsText;
    sf::Font font;
    bool visible;
};
