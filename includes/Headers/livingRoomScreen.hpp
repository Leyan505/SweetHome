#include <iostream>
#include "cScreen.hpp"

class livingRoomScreen : public cScreen
{
private:

public:
	livingRoomScreen(void);
	virtual int Run(sf::RenderWindow &App);
};

livingRoomScreen::livingRoomScreen(void)
{

}

int livingRoomScreen::Run(sf::RenderWindow &App)
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