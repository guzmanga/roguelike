class Engine{
public:

	//list of all actors on the map
	TCODList<Actor *> actors;

	//separate pointer for player ACtor 
	Actor* player;
	Map* map;

	//STARTUP(==0), first frame of the game
	//IDLE(==1), no new turn, redraw the same screen
	//NEW_TURN(==2), update the monster position
	//VICTORY(==3), the player won
	//DEFEAT(==4), the player was killed

	enum GameStatus{
		STARTUP,
		IDLE,
		NEW_TURN,
		VICTORY,
		DEFEAT
	}gameStatus;

	int fovRadius;

	Engine();
	~Engine();
	void update();
	void render();
	void sendToBack(Actor *actor);
	


private:
	bool computeFov;
};


//declaration for global variable named engine
extern Engine engine;