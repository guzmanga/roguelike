#include "libtcod.hpp"
#include "Actor.hpp"
#include "Map.hpp"
#include "Engine.hpp"

Engine::Engine(){
	TCODConsole::initRoot(80, 50, "libtcod C++ tutorial", false);
	player = new Actor(40, 25, '@', TCODColor::white);
	actors.push(player);
	actors.push(new Actor(60, 13, '@', TCODColor::yellow));
	map = new Map(80, 50);
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

		//processes screen redraw events
		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);

		switch(key.vk){
			case TCODK_UP: 
			if(!map->isWall(player->x, player->y-1)){
				player->y--;
			}
			break;
			case TCODK_DOWN:
				if(!map->isWall(player->x, player->y+1)){
					player->y++;
				}
			break;
			case TCODK_LEFT:
				if(!map->isWall(player->x-1, player->y)){
					player->x--;
				}
			break;
			case TCODK_RIGHT:
				if(!map->isWall(player->x+1, player->y)){
					player->x++;
				}
			break;
			default:break;
		}
}

void Engine::render(){
	TCODConsole::root->clear();
	//draw the map
	map->render();

	//draw the actors
	for (Actor **iterator = actors.begin(); iterator != actors.end(); iterator++){
		(*iterator)->render();
	}
}

/*TCODList begin function returns a pointer to the first element, an Actor *
therefore we get a pointer to a pointer to an Actor: Actor**

* indirection operator gets the value of the pointer: Actor * and the 
dereference operator -> access a member (render()) of object being pointed to (Actor)

Map must be drawn before the Actors so that Actors appear on 'top' of the map

*/