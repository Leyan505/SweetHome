#include <iostream>
#include "cScreen.hpp"
#include "GameLogic.hpp"

class bedRoomScreen : public cScreen
{
private:

public:
	bedRoomScreen(void);
	virtual int Run(sf::RenderWindow &App);
};

bedRoomScreen::bedRoomScreen(void)
{
}

int bedRoomScreen::Run(sf::RenderWindow &App)
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