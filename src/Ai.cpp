void MonsterAi::update(Actor *owner){
	//check if actor is living
	if(owner->destructible && owner->destructible->isDead()){
		return;
	}

	if(engine.map->isInFov(owner->x, owner->y)){
		//the player can be seen. move towards him.
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
		//if the destination can be walked, walk to it 
		if(engine.map->canWalk(owner->x+dx,owner->y+dy)){
			owner->x+=dx;
			owner->y+=dy;
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
}

bool PlayerAi::moveOrAttack(Actor *owner, int targetx, int targety){
	if(engine.map->isWall(targetx, targety)) return false;
	//look for living actors to attack
	for(Actor **iterator=engine.actors.begin();
		iterator != engine.actors.end(); iterator++){
		Actor *actor = *iterator;
		if(actor->destructible && !actor->destructible->isDead() && actor->x == targetx && actor->y == target){
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
