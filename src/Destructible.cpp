#include <stdio.h>
#include "main.hpp"

Destructible::Destructible (float maxHp, float defense, const char* corpseName):
	maxHp(maxHp), hp(maxHp), defense(defense), corpseName(corpseName){	
	}

//subtract the defense points and print an appropriate message. if hp is <= 0, then call the die method
float Destructible::takeDamage(Actor *owner, float damage){
	damage-=defense;
	if (damage > 0){
		hp -= damage;
		if (hp <= 0){
			die(owner);
		}
	}else{
		damage = 0;
	}
	return damage;
}


//replace actor character with a bloody red %, change its name to corpse name and set it
//as non blocking; actors are drawn in their orderi n the actor list, so a corpse may be
//drawn on top of a living actor; dead actors are moved to the begining of the list 

void Destructible::die(Actor *owner){
	//transform the actor into a corpose
	owner->ch = '%';
	owner->col=TCODColor::darkRed;
	owner->name=corpseName;
	owner->blocks=false;
	//make sure corpses are drawn before living actors
	engine.sendToBack(owner);
}


MonsterDestructible::MonsterDestructible(float maxHp, float defense, const char *corpseName) :
	Destructible(maxHp, defense, corpseName){
	}

PlayerDestructible::PlayerDestructible(float maxHp, float defense, const char *corpseName) :
	Destructible(maxHp, defense, corpseName){
	}

void MonsterDestructible::die(Actor *owner){
	//transform monster into a corpse
	//printf("%s is dead\n", owner->name);
	engine.gui->message(TCODColor::lightGrey, "%s is dead", owner->name);
	Destructible::die(owner);
}

void PlayerDestructible::die(Actor *owner){
	//printf("You died!\n");
	engine.gui->message(TCODColor::red, "You died!");
	Destructible::die(owner);
	engine.gameStatus=Engine::DEFEAT;
}