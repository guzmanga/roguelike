struct Tile{
	bool canWalk;	//can a tile be walked through?
	Tile() : canWalk(true){}
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

	void setWall(int x, int y);
};

/*struct vs class: field visibility in a struct defaults to public where
in a class it defaults to private. structs generally used for data only 
classes 

map size is not hardcoded; therefore an array of tiles is 
dynamically allocated. tiles field will contain the address 
of the first element of that array

since field is dynamically allocated, it needs to be deleted 
to release memory when Map object is destroyed, hence the destructor*/
