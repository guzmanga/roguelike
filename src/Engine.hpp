class Engine{
public:

	//list of all actors on the map
	TCODList<Actor *> actors;

	//separate pointer for player ACtor 
	Actor* player;
	Map* map;

	Engine();
	~Engine();
	void update();
	void render();
};


//declaration for global variable named engine
extern Engine engine;