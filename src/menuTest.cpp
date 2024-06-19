#include <SFML/Graphics.hpp>
#include <Headers/Menu.h> // Asegúrate de que Menu.h esté en el directorio correcto
#include <iostream>
#include <filesystem.h>


// Clase para manejar la pantalla de créditos
class CreditsScreen {
public:
    CreditsScreen(float width, float height) {
        if (!font.loadFromFile("HalloweenPartySt-14pB.ttf")) {
            std::cout << "ERROR LOADING FONT" << std::endl;
        }

        background.setSize(sf::Vector2f(width, height));
        background.setFillColor(sf::Color(0, 0, 0, 150)); // Fondo semi-transparente

        creditsText.setFont(font);
        creditsText.setFillColor(sf::Color::White);
        creditsText.setCharacterSize(50); // Tamaño de letra más grande
        creditsText.setString("Desarrollado por:\n\nJose Andres Guido Escobar\nEloisse Francesca Molina Camacho\nJorge Isaac Lopez Aragon\nLia Carely Cruz Mendoza");

        // Centrar el texto
        sf::FloatRect textRect = creditsText.getLocalBounds();
        creditsText.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
        creditsText.setPosition(width / 2.0f, height / 2.0f);
    }

    void draw(sf::RenderWindow &window) {
        window.draw(background);
        window.draw(creditsText);
    }

private:
    sf::RectangleShape background;
    sf::Text creditsText;
    sf::Font font;
};

void handleInput(sf::RenderWindow &window, Menu &menu, bool &showCredits) {
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
                if (selectedItem == 4) { // Créditos
                    showCredits = true;
                } else {
                    std::cout << "Selected option: " << selectedItem << std::endl;
                }
            } else if (event.key.code == sf::Keyboard::Escape) {
                if (showCredits) {
                    showCredits = false;
                } else {
                    window.close(); // Cerrar la ventana si se presiona Escape
                }
            }
        }
    }
}

void update() {
    // Aquí puedes agregar cualquier lógica de actualización que necesites
}

void render(sf::RenderWindow &window, Menu &menu, sf::Sprite &background, CreditsScreen &credits, bool showCredits) {
    window.clear();
    window.draw(background); // Dibujar el fondo
    if (showCredits) {
        credits.draw(window); // Dibujar la pantalla de créditos si está activa
    } else {
        menu.draw(window); // Dibujar el menú sobre el fondo
    }
    window.display(); // Mostrar en pantalla
}

int main() {
    // Crear la ventana con el modo de video deseado
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "Game Menu", sf::Style::Close | sf::Style::Fullscreen);

    // Crear el menú con las dimensiones de la ventana
    Menu menu(window.getSize().x, window.getSize().y);

    // Crear la pantalla de créditos con las dimensiones de la ventana
    CreditsScreen credits(window.getSize().x, window.getSize().y);

    // Cargar la textura de fondo desde un archivo
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile(FileSystem::getPath("Resources/Objects/menu/menu.png"))) { // Ruta absoluta al archivo de fondo
    std::cerr << "Error loading background image" << std::endl;
    return -1;
    }

    // Crear el sprite con la textura cargada
    sf::Sprite background(backgroundTexture);

    // Ajustar la escala del fondo para que coincida con el tamaño de la ventana
    float scaleX = static_cast<float>(window.getSize().x) / background.getTexture()->getSize().x;
    float scaleY = static_cast<float>(window.getSize().y) / background.getTexture()->getSize().y;
    background.setScale(scaleX, scaleY);

    bool showCredits = false;

    while (window.isOpen()) {
        handleInput(window, menu, showCredits); // Manejar la entrada del usuario
        update(); // Actualizar la lógica del juego si es necesario
        render(window, menu, background, credits, showCredits); // Renderizar la escena
    }

    return 0;
}
