#include <Headers/CreditsModal.h>
#include <iostream>

CreditsModal::CreditsModal(float width, float height) : visible(false) {
    if (!font.loadFromFile("HalloweenPartySt-14pB.ttf")) {
        std::cout << "ERROR LOADING FONT" << std::endl;
    }

    background.setSize(sf::Vector2f(width * 0.8f, height * 0.6f));
    background.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparente
    background.setPosition(width * 0.1f, height * 0.2f);

    creditsText.setFont(font);
    creditsText.setFillColor(sf::Color::White);
    creditsText.setCharacterSize(30);
    creditsText.setString("Desarrollado por:\n\nTu Nombre\nOtro Nombre\nOtro Nombre");
    creditsText.setPosition(width * 0.15f, height * 0.25f);
}

void CreditsModal::draw(sf::RenderWindow &window) {
    if (visible) {
        window.draw(background);
        window.draw(creditsText);
    }
}

void CreditsModal::show() {
    visible = true;
}

void CreditsModal::hide() {
    visible = false;
}

bool CreditsModal::isVisible() const {
    return visible;
}
