#include "libtcod.hpp"
#include "Map.hpp"


//allocate a Tile array and create two pillars
Map::Map(int width, int height) : width(width), height(height){
	tiles = new Tile[width*height];
	setWall(30, 22);
	setWall(50, 22);
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