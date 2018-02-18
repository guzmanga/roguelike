//Maps are created by digging 'holes' in a map full of walls 

//updating map class to compute the player field of view to light only the visible part of the dungeon 

struct Tile{
	bool explored;	//has the player already seen this tile
	Tile() : explored(false){}	//tiles default to not explored  
};

class Map{
public:
	int width, height;

	Map(int width, int height);
	~Map();
	bool canWalk(int x, int y) const;
	bool isWall(int x, int y) const;
	bool isInFov(int x, int y) const;
	bool isExplored(int x, int y) const;
	void computeFov();
	void render() const;
	void addMonster(int x, int y);  
	//places a monster somewhere on the map

protected:
	Tile* tiles;
	TCODMap *map;
	friend class BspListener;

	void setWall(int x, int y);

	//new dig and createRoom to facilitate room creation
	void dig(int x1, int y1, int x2, int y2);
	void createRoom(bool first, int x1, int y1, int x2, int y2);
};

/*struct vs class: field visibility in a struct defaults to public where
in a class it defaults to private. structs generally used for data only 
classes 

map size is not hardcoded; therefore an array of tiles is 
dynamically allocated. tiles field will contain the address 
of the first element of that array

since field is dynamically allocated, it needs to be deleted 
to release memory when Map object is destroyed, hence the destructor*/
