#include <stdio.h>
#include <math.h>
#include "main.hpp"

static const int TRACKING_TURNS = 3;

void MonsterAi::update(Actor *owner){
	//check if actor is living
	if(owner->destructible && owner->destructible->isDead()){
		return;
	}

	if(engine.map->isInFov(owner->x, owner->y)){
		//the player can be seen. move towards him.
		moveCount = TRACKING_TURNS;
	}else{
		moveCount--;
	}

	if(moveCount > 0){
		moveOrAttack(owner, engine.player->x, engine.player->y);
	}

}

void MonsterAi::moveOrAttack(Actor *owner, int targetx, int targety){
	int dx = targetx - owner->x;
	int dy = targety - owner->y;
	float distance = sqrtf(dx*dx+dy*dy);

	//get an integer deplacement vector by normalizing it (possible values are -1, 0 and 1)
	if(distance >= 2){
		dx=(int)(round(dx/distance));
		dy=(int)(round(dy/distance));
		int stepdx = (dx > 0 ? 1:-1);
   		int stepdy = (dy > 0 ? 1:-1);
		//if the destination can be walked, walk to it 
		if(engine.map->canWalk(owner->x+dx,owner->y+dy)){
			owner->x+=dx;
			owner->y+=dy;
		}else if ( engine.map->canWalk(owner->x+stepdx,owner->y) ) {
           owner->x += stepdx;
       } else if ( engine.map->canWalk(owner->x,owner->y+stepdy) ) {
           owner->y += stepdy;
       }
	}
	//if at melee range and have an attacker feature attack the player 
	else if(owner->attacker){
			owner->attacker->attack(owner,engine.player);
	}
}


void PlayerAi::update(Actor *owner){
	if (owner->destructible && owner->destructible->isDead()){
		return;
	}
	int dx = 0, dy = 0;
		switch(engine.lastKey.vk){
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
			engine.gameStatus=Engine::NEW_TURN;
			if(moveOrAttack(owner, owner->x+dx, owner->y+dy)){
				engine.map->computeFov();
			}
		}
}

bool PlayerAi::moveOrAttack(Actor *owner, int targetx, int targety){
	if(engine.map->isWall(targetx, targety)) return false;
	//look for living actors to attack
	for(Actor **iterator=engine.actors.begin();
		iterator != engine.actors.end(); iterator++){
		Actor *actor = *iterator;
		if(actor->destructible && !actor->destructible->isDead() && actor->x == targetx && actor->y == targety){
			owner->attacker->attack(owner,actor);
			return false; 
		}
	}

	//look for corpses
	for(Actor **iterator=engine.actors.begin();
		iterator != engine.actors.end(); iterator++){
		Actor *actor = *iterator;
		if(actor->destructible && actor->destructible->isDead() && actor->x == targetx && actor->y == targety){
			printf("There is a %s here\n", actor->name);
		}
	}

	owner->x = targetx;
	owner->y=targety;
	return true;
}
