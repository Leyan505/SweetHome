#include <Headers/Menu.h>
#include <iostream>
#include <fstream>

int main()
{
    std::ofstream file("hello.txt");
    auto window = sf::RenderWindow{ { 1920u, 1080u }, "CMake SFML Project" };
    window.setFramerateLimit(144);

    Menu menu(window.getSize().x, window.getSize().y);

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();
        menu.draw(window);
        window.display();
    }
}
