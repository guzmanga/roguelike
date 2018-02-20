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