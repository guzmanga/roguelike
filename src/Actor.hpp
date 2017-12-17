class Actor{
public :
	int x,y; //position on map
	int ch; //ascii code 
	TCODColor col; //color 

	Actor(int x, int y, int ch, const TCODColor &col);
	//const means that function does not modify content of Actor (func can be called on constant objects)
	void render() const; 

};