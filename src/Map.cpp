#include <stdio.h>
#include "main.hpp"

/*static keyword when used on a global variables means variable is not visible
from outside the .cpp file*/ 

static const int ROOM_MAX_SIZE = 12;
static const int ROOM_MIN_SIZE = 6;
static const int MAX_ROOM_MONSTERS = 3;
static const int MAX_ROOM_ITEMS = 2;


/*class that inherits class ITCODBspCallback (declared in libtcod)
visitNode must be defined as it is specific to what must be done at each node 
in this case it is creating a room */
class BspListener : public ITCODBspCallback{
private:
	Map &map; //a map to dig
	int roomNum; //room number
	int lastx, lasty; //center of the last room 

public: 
	BspListener(Map &map) : map(map), roomNum(0){}

	/*called by traverseInvertedLevelOrder as a callback function during traversal of BSP
	  takes a TCODBsp pointer that contains x,y,w,h parameters as well as the position of splitting 
	*/
	bool visitNode(TCODBsp *node, void *userData){
		if( node->isLeaf()){
			int x,y,w,h;

			//digging a room
			//first set the size of the node 
			TCODRandom *rng = TCODRandom::getInstance();
			w = rng->getInt(ROOM_MIN_SIZE, node->w-2);
			h = rng->getInt(ROOM_MIN_SIZE, node->h-2);
			x = rng->getInt(node->x+1, node->x + node->w-w-1);
			y = rng->getInt(node->y+1, node->y + node->h-h-1);

			//must also draw the node on the map
			map.createRoom(roomNum == 0, x ,y , x+w-1, y+h-1);

			//come back to this and understand it better (the math part)
			if (roomNum != 0){
				//dig a corridor from last room
				map.dig(lastx, lasty, x+w/2, lasty);
				map.dig(x+w/2, lasty, x+w/2, y+h/2);
			}

		lastx = x + w/2;
		lasty = y + h/2;
		roomNum++;
		}
		return true;
	}
};

//allocate a Tile array and create two pillars
Map::Map(int width, int height) : width(width), height(height){
	tiles = new Tile[width*height];	//array of Tile objects
	map = new TCODMap(width, height);
	//bsp = BINARY SPACE PARTITION 
	TCODBsp bsp(0, 0, width, height);	//create bsp object with same size as map
	
	//split object so that every node is at least max sie of rooms 
	bsp.splitRecursive(NULL, 8, ROOM_MAX_SIZE, ROOM_MAX_SIZE, 1.5, 1.5);
	BspListener listener(*this);	//*this refers to the Map instance that is created
	bsp.traverseInvertedLevelOrder(&listener, NULL);
}


//releases everything that was allocated in constructor 
Map::~Map(){
	delete [] tiles;
	delete map;
}


//function to detect if a Tile is occupied by another actor already
bool Map::canWalk(int x, int y)const{
	if (isWall(x, y)){
		//this is a wall
		return false;
	}
	for (Actor **iterator=engine.actors.begin();
		iterator != engine.actors.end();
		iterator++){
		Actor *actor = *iterator;
	if(actor->blocks && actor->x == x && actor->y == y){
		//an actor exists at this point; cannot walk here
		return false;
		}
	}

	return true;
}

bool Map::isWall(int x, int y)const{
	return !map->isWalkable(x,y);
}

bool Map::isExplored(int x, int y) const{
	return tiles[x+y*width].explored;
}

// void Map::setWall(int x, int y){
// 	tiles[x + y * width].canWalk = false;
// }

bool Map::isInFov(int x, int y)const{
	if(x < 0 || x >= width || y < 0 || y >= height){
		return false;
	}
	if(map->isInFov(x,y)){
		tiles[x+y*width].explored=true;
		return true;
	}
	return false;
}

void Map::computeFov(){
	map->computeFov(engine.player->x, engine.player->y, engine.fovRadius);
}


/*static keyword means variable won't be recreated every call to render 
const helps compiler optimize code by telling it contents of objects won't be modified

ternary conditional operator ?:
equivalent:
	if(isWall(x,y)){
		TCODConsole::root->setCharBackground(x,y,darkWall);
	}
	else{
		TCODConsole::root->setCharBackground(x,y,darkGround);
	}
*/
void Map::render() const{
	static const TCODColor darkWall(0,0,100);
	static const TCODColor darkGround(50,50,150);
	static const TCODColor lightWall(130, 110, 50);
	static const TCODColor lightGround(200, 180, 50);

	for (int x=0; x < width; x++){
		for(int y = 0; y < height; y++){

			if(isInFov(x,y)){
				TCODConsole::root->setCharBackground(x,y,
					isWall(x,y) ?lightWall : lightGround);
			}
			else if(isExplored(x,y)){
				TCODConsole::root->setCharBackground(x,y,
					isWall(x,y) ? darkWall : darkGround);
			}
		}
	}
}

void Map::dig(int x1, int y1, int x2, int y2){
	if (x2 < x1){
		int temp = x2;
		x2 = x1;
		x1 = temp;
	}

	if (y2 < y1){
		int temp = y2;
		y2 = y1;
		y1 = temp;
	}

	for(int tilex = x1; tilex <= x2; tilex++){
		for(int tiley = y1; tiley <= y2; tiley++){
			//???????????how to access a range of elements in '2d' array
			map->setProperties(tilex,tiley,true,true);
		}
	}
}


/*dig room then put player in its center if its the first room
or some NPC in 25% of other rooms using libcotd's random number generator */

void Map::createRoom(bool first, int x1, int y1, int x2, int y2){
	dig(x1, y1, x2, y2);
	if(first){
		//put the player in the first room
		engine.player->x = (x1 + x2)/2;
		engine.player->y = (y1 + y2)/2;
	}

//creates an orc 80% of the time or a troll
//get a random number of monsters (ceiling is MAX_MONSTERS) and for each one
//get an empty random tile and place the monster there
	else{
		TCODRandom *rng = TCODRandom::getInstance();
		int nbMonsters = rng->getInt(0, MAX_ROOM_MONSTERS);
		while(nbMonsters > 0){
			int x = rng->getInt(x1, y2);
			int y = rng->getInt(y1, y2);
			if(canWalk(x, y)){
				addMonster(x,y);
			}
			nbMonsters--;
		}

		int nbItems = rng->getInt(0, MAX_ROOM_ITEMS);
		while(nbItems > 0){
			int x = rng->getInt(x1, x2);
			int y = rng->getInt(y1, y2);
			if(canWalk(x, y)){
				addItem(x,y);
			}
			nbItems--;
		}
	}
}




void Map::addMonster(int x, int y){
	TCODRandom *rng = TCODRandom::getInstance();
	if(rng->getInt(0, 100) < 80){
		//create an orc
		Actor *orc = new Actor(x,y,'o',"orc",TCODColor::desaturatedGreen);
		orc->destructible = new MonsterDestructible(10,0,"dead orc");
		orc->attacker = new Attacker(3);
		orc->ai = new MonsterAi();
		engine.actors.push(orc);
	}else{
		//create a troll;
		Actor *troll = new Actor(x,y,'T',"troll",TCODColor::darkerGreen);
		troll->destructible = new MonsterDestructible(10,0,"dead orc");
		troll->attacker = new Attacker(4);
		troll->ai = new MonsterAi();
		engine.actors.push(troll);
	}
}

void Map::addItem(int x, int y){
	Actor *healthPotion = new Actor(x,y, '!', "health potion", TCODColor::violet);
	healthPotion->blocks = false;
	healthPotion->pickable = new Healer(4);
	engine.actors.push(healthPotion);
}