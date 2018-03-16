class Container{
public:
	int size; //max number of actors 
	TCODList<Actor *>inventory;

	Container(int size);
	~Container();
	bool add(Actor *actor);
	void remove(Actor *actor);
};

