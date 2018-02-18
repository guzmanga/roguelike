#include <stdio.h>
#include "libtcod.hpp"
#include "Actor.hpp"
#include "Map.hpp"
#include "Engine.hpp"

//constructor uses initializattion list to define value of class' members
Actor::Actor(int x, int y, int ch, const char *name, const TCODColor &col) :
	x(x), y(y), ch(ch), col(col), name(name){

	}

	void Actor::render() const{
		TCODConsole::root->setChar(x, y , ch);
		TCODConsole::root->setCharForeground(x,y,col);
	}

	void Actor::update(){
		printf("The %s growls!\n", name);
	}

	bool Actor::moveOrAttack(int x, int y){
		if (engine.map->isWall(x,y))
			return false;
		for(Actor **iterator=engine.actors.begin();
			iterator != engine.actors.end();
			iterator++){
			Actor *actor = *iterator;
		if( actor->x == x && actor->y == y){
			printf("the %s laughs at your puny efforts to attack him!\n", actor->name);
			return false;
		}
		}
		this->x=x;
		this->y=y;
		return true;
	}