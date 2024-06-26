#include <Headers/MenuModal.h>
#include <iostream>


MenuModal::MenuModal(unsigned int width, unsigned int height)
    : showModal(false), selectedOption(0), windowWidth(width), windowHeight(height)
{
    options[0] = "Option 1";
    options[1] = "Option 2";
    options[2] = "Option 3";

    modalBackground.setSize(sf::Vector2f(windowWidth * 0.5f, windowHeight * 0.5f));
    modalBackground.setFillColor(sf::Color(50, 50, 50, 200));
    modalBackground.setPosition(windowWidth * 0.25f, windowHeight * 0.25f);

    if (!font.loadFromFile("arial.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
    }
}

void MenuModal::render(sf::RenderWindow& window)
{
    window.pushGLStates();
    window.resetGLStates();

    window.draw(modalBackground);

    for (int i = 0; i < 3; ++i)
    {
        sf::Text text;
        text.setFont(font);
        text.setString(options[i]);
        text.setCharacterSize(24);
        text.setFillColor(i == selectedOption ? sf::Color::Red : sf::Color::White);
        text.setPosition(windowWidth * 0.3f, windowHeight * 0.35f + i * 50);
        window.draw(text);
    }

    window.popGLStates();
}

void MenuModal::processInput(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Space)
            toggleVisibility();

        if (showModal)
        {
            if (event.key.code == sf::Keyboard::Up)
                selectedOption = (selectedOption - 1 + 3) % 3;
            if (event.key.code == sf::Keyboard::Down)
                selectedOption = (selectedOption + 1) % 3;
            if (event.key.code == sf::Keyboard::Enter)
            {
                std::cout << "Selected: " << options[selectedOption] << std::endl;
                showModal = false; // Close the modal after selection
            }
        }
    }
}