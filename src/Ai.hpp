//pure virtual method, or an abstract method;
//Ai class has no implementation for htis method therefore it is an 
//abstract class that can't be instantiated 

class Ai{
public:
	virtual void update(Actor *owner)=0;
};


class PlayerAi : public Ai{
public:
	void update(Actor *owner);

protected:
	bool moveOrAttack(Actor *owner, int targetx, int targety);
};

class MonsterAi : public Ai{
public:
	void update(Actor *owner);

protected:
	void moveOrAttack(Actor *owner, int targetx, int targety);
	int moveCount;
};
