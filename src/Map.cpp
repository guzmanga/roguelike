#include "libtcod.hpp"
#include "Map.hpp"
#include "Actor.hpp"
#include "Engine.hpp"

/*static keyword when used on a global variables means variable is not visible
from outside the .cpp file*/ 

static const int ROOM_MAX_SIZE = 12;
static const int ROOM_MIN_SIZE = 6;



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
}

bool Map::isWall(int x, int y)const{
	return !tiles[x+y*width].canWalk;
}

void Map::setWall(int x, int y){
	tiles[x + y * width].canWalk = false;
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

	for (int x=0; x < width; x++){
		for(int y = 0; y < height; y++){
			TCODConsole::root->setCharBackground(x,y,
				isWall(x,y) ? darkWall : darkGround);
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
			tiles[tilex+tiley*width].canWalk = true; 
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
	else{
		TCODRandom *rng = TCODRandom::getInstance();
		if(rng->getInt(0, 3) == 0){
			engine.actors.push(new Actor((x1 + x2)/2, (y1 + y2) /2, '@', 
				TCODColor::yellow));
		}
	}
}
