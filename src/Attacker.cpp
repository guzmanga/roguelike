#include <stdio.h>
#include "main.hpp"

Attacker::Attacker(float power): power(power){
}

void Attacker::attack(Actor *owner, Actor *target){
	if (target->destructible && ! target->destructible->isDead()){
		if(power - target->destructible->defense > 0){
			//printf("%s attacks %s for %g hit points.\n", owner->name, target->name, power - target->destructible->defense);
			engine.gui->message(owner==engine.player ? TCODColor::red : TCODColor::lightGrey,
				"%s attacks %s for %g hit points.", owner->name, target->name, power - target->destructible->defense);
		}else{
			//printf("%s attacks %s but it has no effect!\n", owner->name, target->name);
			engine.gui->message(TCODColor::lightGrey, "%s attacks %s but it has no effect!", owner->name, target->name);
		}
		target->destructible->takeDamage(target, power);
	}else{
		//printf("%s attacks %s in vain. \n", owner->name, target->name);
		engine.gui->message(TCODColor::lightGrey, "%s attacks %s in vain.", owner->name, target->name);
	}
}