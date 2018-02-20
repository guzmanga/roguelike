#include <stdio.h>
#include "main.hpp"

//define instance of global variable engine
Engine engine(80,50);

int main(){


	//start main game loop and keep it open until window is closed by player 
	while( !TCODConsole::isWindowClosed()){
		engine.update();
		engine.render();
		TCODConsole::flush();
	}

	return 0;

}