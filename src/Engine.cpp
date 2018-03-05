#include <stdio.h>
#include "main.hpp"

Engine::Engine(int screenWidth, int screenHeight) : gameStatus(STARTUP), fovRadius(10),
	screenWidth(screenWidth), screenHeight(screenHeight){
	TCODConsole::initRoot(screenWidth, screenHeight, "libtcod C++ tutorial", false);
	player = new Actor(40, 25, '@', "player", TCODColor::white);
	player->destructible=new PlayerDestructible(30, 2,"your cadaver");
	player->attacker = new Attacker(5);
	player->ai = new PlayerAi();
	actors.push(player);
	map = new Map(80, 43);
	gui = new Gui();
	gui->message(TCODColor::red, "Welcome stranger! Prepare yourself!");
}

/*dynamically allocate two Actor objects one for the player and one for some NPC 

the object address are stored in the actors list 

Map is also dynamically allocated

note: Actors have not been placed in the Map yet
*/

Engine::~Engine(){
	actors.clearAndDelete();
	delete map;
	delete gui;
}

void Engine::update(){
	if (gameStatus == STARTUP) map->computeFov();
	gameStatus = IDLE;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE,&lastKey,&mouse);
	player->update();
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
	player->render();
	//show player stats
	gui->render();
	TCODConsole::root->print(1,screenHeight-2, "HP: %d/%d",
		(int)player->destructible->hp,(int)player->destructible->maxHp);
}

/*TCODList begin function returns a pointer to the first element, an Actor *
therefore we get a pointer to a pointer to an Actor: Actor**

* indirection operator gets the value of the pointer: Actor * and the 
dereference operator -> access a member (render()) of object being pointed to (Actor)

Map must be drawn before the Actors so that Actors appear on 'top' of the map

*/


void Engine::sendToBack(Actor *actor){
	actors.remove(actor);
	actors.insertBefore(actor, 0);
}