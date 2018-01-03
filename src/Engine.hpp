class Engine{
public:

	//list of all actors on the map
	TCODList<Actor *> actors;

	//separate pointer for player ACtor 
	Actor* player;
	Map* map;

	int fovRadius;

	Engine();
	~Engine();
	void update();
	void render();

private:
	bool computeFov;
};


//declaration for global variable named engine
extern Engine engine;