#include <Headers/Menu.h>
#include <iostream>

// Constructor del menú con parámetros
Menu::Menu(float width, float height) {
    if (!font.loadFromFile("HalloweenPartySt-14pB.ttf")) {
        std::cout << "ERROR LOADING FONT" << std::endl;
    }

    // Configuración inicial de las opciones del menú
    for (int i = 0; i < MAX_NUMBER_OPTIONS; ++i) {
        option[i].setFont(font);
        option[i].setFillColor(sf::Color::White);
        option[i].setCharacterSize(50); // Tamaño de letra más grande
        // Ajuste de la posición para estar más a la izquierda y más juntos verticalmente
        option[i].setPosition(sf::Vector2f(100, height / (MAX_NUMBER_OPTIONS + 1) * (i + 1) * 0.5f + 300)); // Desplazamiento adicional
    }

    // Nombres de las opciones del menú
    option[0].setString("Living room");
    option[1].setString("Kitchen");
    option[2].setString("Bedroom");
    option[3].setString("Hallway");
    option[4].setString("House");
    option[5].setString("Credits");

    selectedOpt = 0;
    option[selectedOpt].setFillColor(sf::Color::Yellow); // Opción seleccionada en amarillo
    selectSound = nullptr; // Inicializar el puntero de sonido
}

// Constructor por defecto
Menu::Menu() : Menu(800, 600) {}

// Función para establecer el sonido de selección
void Menu::setSelectSound(sf::Sound &sound) {
    selectSound = &sound;
}

// Función para dibujar el menú
void Menu::draw(sf::RenderWindow &window) {
    for (int i = 0; i < MAX_NUMBER_OPTIONS; ++i) {
        window.draw(option[i]);
    }
}

// Función para mover la selección hacia arriba
void Menu::moveUp() {
    if (selectedOpt - 1 >= 0) {
        option[selectedOpt].setFillColor(sf::Color::White);
        selectedOpt--;
        option[selectedOpt].setFillColor(sf::Color::Yellow);
        if (selectSound) {
            selectSound->play();
        }
    }
}

// Función para mover la selección hacia abajo
void Menu::moveDown() {
    if (selectedOpt + 1 < MAX_NUMBER_OPTIONS) {
        option[selectedOpt].setFillColor(sf::Color::White);
        selectedOpt++;
        option[selectedOpt].setFillColor(sf::Color::Yellow);
        if (selectSound) {
            selectSound->play();
        }
    }
}

// Obtener la opción seleccionada
int Menu::getPressedItem() {
    return selectedOpt;
}
