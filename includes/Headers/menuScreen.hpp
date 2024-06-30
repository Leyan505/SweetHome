#include <Headers/Menu.h> // Asegúrate de que Menu.h esté en el directorio correcto
#include "cScreen.hpp"
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


int handleInput(sf::RenderWindow &window, Menu &menu, bool &showCredits, bool &first) {
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
                switch (selectedItem)
                {
                case 3:
                    if(!first) {
                        window.popGLStates();
                        }
                    else
                    {
                        first = false;
                    }
                    return (4);
                    break;
                case 4:
                    showCredits = true;
					return 0;
                default:
                    break;
                }
            } else if (event.key.code == sf::Keyboard::Escape) {
                if (showCredits) {
                    showCredits = false;
                } else {
                    return (-1); // Cerrar la ventana si se presiona Escape
                }
            }
        }
    }
    return 0;
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

class menuScreen : public cScreen
{
private:
	int alpha_max;
	int alpha_div;
	bool playing;
    bool first;
public:
	menuScreen(void);
	virtual int Run(sf::RenderWindow &App);
};

menuScreen::menuScreen(void)
{
	alpha_max = 3 * 255;
	alpha_div = 3;
	playing = false;
    first = true;
}

int menuScreen::Run(sf::RenderWindow &App)
{
	bool Running = true;
	int alpha = 0;

	// Crear el menú con las dimensiones de la ventana
    Menu menu(App.getSize().x, App.getSize().y);

    // Crear la pantalla de créditos con las dimensiones de la ventana
    CreditsScreen credits(App.getSize().x, App.getSize().y);

    // Cargar la textura de fondo desde un archivo
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile(FileSystem::getPath("Resources/Objects/menu/menu.png"))) { // Ruta absoluta al archivo de fondo
    std::cerr << "Error loading background image" << std::endl;
    return -1;
    }

    // Crear el sprite con la textura cargada
    sf::Sprite background(backgroundTexture);

    // Ajustar la escala del fondo para que coincida con el tamaño de la ventana
    float scaleX = static_cast<float>(App.getSize().x) / background.getTexture()->getSize().x;
    float scaleY = static_cast<float>(App.getSize().y) / background.getTexture()->getSize().y;
    background.setScale(scaleX, scaleY);

    bool showCredits = false;

	if (playing)
	{
		alpha = alpha_max;
	}
	int screen;
	while (Running)
	{
		screen = handleInput(App, menu, showCredits, first);
		if(screen != 0 )
		{
			return screen;
		}; // Manejar la entrada del usuario
        render(App, menu, background, credits, showCredits); // Renderizar la escena
        
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}