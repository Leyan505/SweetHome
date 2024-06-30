#include <iostream>
#include "cScreen.hpp"

class kitchenScreen : public cScreen
{
private:

public:
	kitchenScreen(void);
	virtual int Run(sf::RenderWindow &App);
};

kitchenScreen::kitchenScreen(void)
{
}

int kitchenScreen::Run(sf::RenderWindow &App)
{
	sf::Event Event;
	bool Running = true;

	while (Running)
	{
		//Verifying events
		while (App.pollEvent(Event))
		{
			
		}
	}

	//Never reaching this point normally, but just in case, exit the application
	return -1;
}