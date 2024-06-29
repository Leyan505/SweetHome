#ifndef MENU_MODAL_H
#define MENU_MODAL_H

#include <SFML/Graphics.hpp>
#include <iostream>

class MenuModal
{
public:
    MenuModal(unsigned int width, unsigned int height);
    void render(sf::RenderWindow& window);
  #ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <iostream>

#define MAX_NUMBER_OF_ITEMS 3

class Menu {
public:
    Menu(float width, float height);
    ~Menu();

    void draw(sf::RenderWindow &window);
    void moveUp();
    void moveDown();
    int getPressedItem() { return selectedItemIndex; }

private:
    int selectedItemIndex;
    sf::Font font;
    sf::Text menu[MAX_NUMBER_OF_ITEMS];
};

#endif
  void processInput(sf::Event event);

    bool isVisible() const { return showModal; }
    void toggleVisibility() { showModal = !showModal; }
    const char* getSelectedOption() const { return options[selectedOption]; }

private:
    bool showModal;
    int selectedOption;
    const char* options[3];

    unsigned int windowWidth;
    unsigned int windowHeight;

    sf::RectangleShape modalBackground;
    sf::Font font;
};

#endif // MENU_MODAL_H