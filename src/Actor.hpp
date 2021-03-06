class Actor{
public :
	int x,y; //position on map
	int ch; //ascii code 
	TCODColor col; //color 
	const char *name; //actor name
	bool blocks; //can an actor be walked on
	Attacker *attacker; //something that deals damage
	Destructible *destructible; //something that can be damaged
	Ai *ai; //smoething self updating
	Pickable *pickable;	//can be picked up and used 
	Container *container; //can contain actors

	Actor(int x, int y, int ch, const char *name, const TCODColor &col);
	~Actor();
	//const means that function does not modify content of Actor (func can be called on constant objects)
	void update(); //handles monster turn
	void render() const; 
	float getDistance(int cx, int cy) const;

	//bool moveOrAttack(int x, int y); //handles player turn; true if player actually moved, false if a wall or monster is hit
	

};