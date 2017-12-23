//Maps are created by digging 'holes' in a map full of walls 

struct Tile{
	bool canWalk;	//can a tile be walked through?
	Tile() : canWalk(false){}	//tiles default to non-walking 
};

class Map{
public:
	int width, height;

	Map(int width, int height);
	~Map();
	bool isWall(int x, int y) const;
	void render() const;

protected:
	Tile* tiles;
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
