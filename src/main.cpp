#include <iostream>
#include "glad.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"

#include <filesystem.h>
#include <Headers/shader_m.h>
#include <Headers/Camera.h>
#include <Headers/model.h>
#include <Headers/screens.hpp>


int main(int argc, char** argv)
{
	//Applications variables
	std::vector<cScreen*> Screens;
	int screen = 0;

	//Window creation
	sf::RenderWindow App(desktopMode, "SweetHome", sf::Style::Default, sf::ContextSettings(24));
    App.setFramerateLimit(165);
    App.setActive(true);
    App.setMouseCursorGrabbed(true);
    App.setMouseCursorVisible(false);

	if (!gladLoadGLLoader((GLADloadproc)sf::Context::getFunction))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

	//Screens preparations
	menuScreen s0;
	Screens.push_back(&s0);
	livingRoomScreen s1;
	Screens.push_back(&s1);
	kitchenScreen s2;
	Screens.push_back(&s2);
	bedRoomScreen s3;
	Screens.push_back(&s3);
	corridorScreen s4;
	Screens.push_back(&s4);

	//Main loop
	while (screen >= 0)
	{
		screen = Screens[screen]->Run(App);
	}

	return EXIT_SUCCESS;
}