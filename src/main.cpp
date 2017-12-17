#include "libtcod.hpp"
#include "Actor.hpp"
#include "Map.hpp"
#include "Engine.hpp"

//define instance of global variable engine
Engine engine;

int main(){


	//start main game loop and keep it open until window is closed by player 
	while( !TCODConsole::isWindowClosed()){
		engine.update();
		engine.render();
		TCODConsole::flush();
	}

	return 0;

}