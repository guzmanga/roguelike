class Destructible{
public:
	float maxHp; //max hp
	float hp; //current health points
	float defense; //hitpoints deflected
	const char *corpseName; //name once dead/destroyed

	Destructible(float maxHp, float defense, const char *corpseName);
	inline bool isDead(){return hp <= 0;}	//inline keyword tells compiler to replace calls to this function directly by the function code 

	//handles damage given to the Destructible; owner is the actor targetted by the attack, the one containing 
	//the Destructible class; function returns the number of hit points actually taken(damage - defense)
	float takeDamage(Actor *owner, float damage);	

	//takes care of what happens when hp reaches 0 
	virtual void die(Actor *owner);

};


class MonsterDestructible : public Destructible{
public:
	MonsterDestructible(float maxHp, float defense, const char *corpseName);
	void die(Actor *owner);
};

class PlayerDestructible : public Destructible{
public:
	PlayerDestructible(float maxHp, float defense, const char *corpseName);
	void die(Actor *owner);
};