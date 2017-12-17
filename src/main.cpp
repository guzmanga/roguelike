#include "libtcod.hpp"

int main(){

	int playerx = 40, playery = 25;

	//create game window false indicates no fullscreen mode 
	TCODConsole::initRoot(80,50, "libtcod C++ tutorial", false);

	//start main game loop and keep it open until window is closed by player 
	while( !TCODConsole::isWindowClosed()){

		//key handles input
		TCOD_key_t key;

		//processes screen redraw events
		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);

		switch(key.vk){
			case TCODK_UP: 
				playery--;
				break;
			case TCODK_DOWN:
				playery++;
				break;
			case TCODK_LEFT:
				playerx--;
				break;
			case TCODK_RIGHT:
				playerx++;
				break;
			default:
				break;
		}

		//pointer to console bound to main game window; clear 
		//erases everything with default color
		TCODConsole::root->clear();

		//draws char in the playerx, playery using default color white
		TCODConsole::root->putChar(playerx, playery, '@');

		//generates console image and displays it on window 
		TCODConsole::flush();
	}

	return 0;

}