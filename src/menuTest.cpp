#include <SFML/Graphics.hpp>
#include <Headers/Menu.h> // Asumiendo que Menu.h está en el directorio actual
#include <iostream>
#include <filesystem.h>

void handleInput(sf::RenderWindow &window, Menu &menu) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) {
                menu.moveUp();
            } else if (event.key.code == sf::Keyboard::Down) {
                menu.moveDown();
            } else if (event.key.code == sf::Keyboard::Return) {
                int selectedItem = menu.getPressedItem();
                std::cout << "Selected option: " << selectedItem << std::endl;
                // Aquí puedes manejar la opción seleccionada, como iniciar un juego, abrir una configuración, etc.
            } else if (event.key.code == sf::Keyboard::Escape) {
                window.close(); // Cerrar la ventana si se presiona Escape
            }
        }
    }
}

void update() {
    // Aquí puedes agregar cualquier lógica de actualización que necesites
}

void render(sf::RenderWindow &window, Menu &menu, sf::Sprite &background) {
    window.clear();
    window.draw(background); // Dibujar el fondo
    menu.draw(window); // Dibujar el menú sobre el fondo
    window.display(); // Mostrar en pantalla
}

int main() {
    // Crear la ventana con el modo de video deseado
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "Game Menu", sf::Style::Close | sf::Style::Fullscreen);

    // Crear el menú con las dimensiones de la ventana
    Menu menu(window.getSize().x, window.getSize().y);

    // Cargar la textura de fondo desde un archivo
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile(FileSystem::getPath("Resources/Objects/menu/wallpaper.jpg"))) { // Ruta absoluta al archivo de fondo
        std::cerr << "Error loading background image" << std::endl;
        return -1;
    }

    // Crear el sprite con la textura cargada
    sf::Sprite background(backgroundTexture);

    // Ajustar la escala del fondo para que coincida con el tamaño de la ventana
    float scaleX = static_cast<float>(window.getSize().x) / background.getTexture()->getSize().x;
    float scaleY = static_cast<float>(window.getSize().y) / background.getTexture()->getSize().y;
    background.setScale(scaleX, scaleY);

    while (window.isOpen()) {
        handleInput(window, menu); // Manejar la entrada del usuario
        update(); // Actualizar la lógica del juego si es necesario
        render(window, menu, background); // Renderizar la escena
    }

    return 0;
}
