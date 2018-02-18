#include "libtcod.hpp"
#include "Actor.hpp"
#include "Map.hpp"
#include "Engine.hpp"

Engine::Engine() : gameStatus(STARTUP), fovRadius(10){
	TCODConsole::initRoot(80, 50, "libtcod C++ tutorial", false);
	player = new Actor(40, 25, '@', "player", TCODColor::white);
	actors.push(player);
	map = new Map(80, 45);
}

/*dynamically allocate two Actor objects one for the player and one for some NPC 

the object address are stored in the actors list 

Map is also dynamically allocated

note: Actors have not been placed in the Map yet
*/

Engine::~Engine(){
	actors.clearAndDelete();
	delete map;
}

void Engine::update(){
	TCOD_key_t key;
	if (gameStatus == STARTUP) map->computeFov();
	gameStatus = IDLE;
		//processes screen redraw events
		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
		int dx = 0, dy = 0;
		switch(key.vk){
			case TCODK_UP: 
			dy=-1;
			break;
			case TCODK_DOWN:
			dy=1;
			break;
			case TCODK_LEFT:
			dx=-1;
			break;
			case TCODK_RIGHT:
			dx=1;
			break;
			default:break;
		}

		//some movement key was pressed, and attempt to move the player
		//if successful, recompue the FOV

		if(dx != 0 || dy != 0){
			gameStatus=NEW_TURN;
			if(player->moveOrAttack(player->x+dx, player->y+dy)){
				map->computeFov();
			}
		}

		if (gameStatus == NEW_TURN){
			for(Actor **iterator=actors.begin(); iterator !=actors.end();
				iterator++){
				Actor *actor = *iterator;
				if(actor != player){
					actor->update();
				}
			}
		}
}

void Engine::render(){
	TCODConsole::root->clear();
	//draw the map
	map->render();

	//draw the actors only if they are in the FOV
	for (Actor **iterator = actors.begin(); iterator != actors.end(); iterator++){
		Actor *actor=*iterator;
		if(map->isInFov(actor->x, actor->y)){
			actor->render();
		}
	}
}

/*TCODList begin function returns a pointer to the first element, an Actor *
therefore we get a pointer to a pointer to an Actor: Actor**

* indirection operator gets the value of the pointer: Actor * and the 
dereference operator -> access a member (render()) of object being pointed to (Actor)

Map must be drawn before the Actors so that Actors appear on 'top' of the map

*/