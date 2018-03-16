#include <stdio.h>
#include <math.h>
#include "main.hpp"

//constructor uses initializattion list to define value of class' members
Actor::Actor(int x, int y, int ch, const char *name, const TCODColor &col) :
	x(x), y(y), ch(ch), col(col), name(name), blocks(true), attacker(NULL), destructible(NULL), ai(NULL),
	pickable(NULL), container(NULL){

	}

	void Actor::render() const{
		TCODConsole::root->setChar(x, y , ch);
		TCODConsole::root->setCharForeground(x,y,col);
	}

	//integer in C is false if == 0, else true

	void Actor::update(){
		printf("The %s growls!\n", name);
		if(ai)ai->update(this);
	}

	Actor::~Actor(){
		if(attacker)delete attacker;
		if(destructible)delete destructible;
		if(ai) delete ai;
		if(pickable) delete pickable;
		if(container) delete container;
	}

	float Actor::getDistance(int cx, int cy) const{
		int dx=x-cx;
		int dy=y-cy;
		return sqrt(dx*dx+dy*dy);
	}

	/*bool Actor::moveOrAttack(int x, int y){
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
	}*/