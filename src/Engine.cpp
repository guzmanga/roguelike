#include <stdio.h>
#include "main.hpp"

Engine::Engine(int screenWidth, int screenHeight) : gameStatus(STARTUP), fovRadius(10),
	screenWidth(screenWidth), screenHeight(screenHeight){
	TCODConsole::initRoot(screenWidth, screenHeight, "libtcod C++ tutorial", false);
	player = new Actor(40, 25, '@', "player", TCODColor::white);
	player->destructible=new PlayerDestructible(30, 2,"your cadaver");
	player->attacker = new Attacker(5);
	player->ai = new PlayerAi();
	player->container = new Container(26);
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

Actor *Engine::getClosestMonster(int x, int y, float range) const{
	Actor *closest=NULL;
	float bestDistance=1E6f;

	for(Actor **iterator=actors.begin();
		iterator != actors.end(); iterator++){
		Actor *actor = *iterator;
	if(actor != player && actor->destructible 
		&& !actor->destructible->isDead()){
		float distance = actor->getDistance(x,y);
	if(distance < bestDistance && (distance <= range || range == 0.0f)){
		bestDistance=distance;
		closest=actor;
	}
	}
}
return closest;
}

bool Engine::pickATile(int *x, int *y, float maxRange){
	while(!TCODConsole::isWindowClosed()){
		render();
		//highlight the possible range
		for(int cx = 0; cx < map->width; cx++){
			for(int cy=0; cy < map->height; cy++){
				if(map->isInFov(cx,cy) && (maxRange == 0 || player->getDistance(cx, cy) <= maxRange)){
					TCODColor col = TCODConsole::root->getCharBackground(cx, cy);
					col = col * 1.2f;
					TCODConsole::root->setCharBackground(cx,cy,col);
				}
			}
		}
		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE, &lastKey, &mouse);
		if(map->isInFov(mouse.cx, mouse.cy) && (maxRange == 0 || player->getDistance(mouse.cx, mouse.cy) <= maxRange)){
			TCODConsole::root->setCharBackground(mouse.cx, mouse.cy, TCODColor::white);
			if(mouse.lbutton_pressed){
				*x=mouse.cx;
				*y=mouse.cy;
				return true;
			}
		}
		if(mouse.rbutton_pressed || lastKey.vk != TCODK_NONE){
			return false;
		}
		TCODConsole::flush();
	}
	return false;
}

Actor *Engine::getActor(int x, int y)const{
	for(Actor **iterator=actors.begin();
		iterator != actors.end(); iterator++){
		Actor *actor = *iterator;
	if(actor->x == x && actor->y == y && actor->destructible && !actor->destructible->isDead()){
		return actor;
	}
	}
	return NULL;
}