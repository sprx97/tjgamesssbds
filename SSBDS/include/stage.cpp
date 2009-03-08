class Ledge {
	public:
		int x, y; // x and y position of the ledge
		string direction;
		Ledge(int xpos, int ypos, string dir) {
			x = xpos;
			y = ypos;
			direction = dir;
		} // creates a new ledge
}; // a ledge on the stage
class Floor {
	public:
		int x, y; // x and y of far left of stage
		int length; // total run of the floor
		bool isplatform; // is a platform of not
		vector<double> slopes; // sloping of the stage for each x position
		Floor(int xpos, int ypos, int l, bool plat) {
			x = xpos;
			y = ypos;
			length = l;
			isplatform = plat;
		} // creates a new floor
		bool isPlatform() { return isplatform; } // returns isplatform
		double totalrise() {
			double total = 0;
			for(int n = 0; n < (int)slopes.size(); n++) {
				total += slopes[n];
			}
			return total;
		} // counts the total rise of the stage from start to finish
		double getrise(int distance) {
			double total = 0;
			for(int n = x; n < distance; n++) {
				total += slopes[n-x];
			}
			return total;
		} // the rise up to distance distance
}; // a floor on the stage
class Ceiling {
	public:
		int x, y; // x and y position of ceiling
		int length; // length of ceiling
		Ceiling(int xpos, int ypos, int l) {
			x = xpos;
			y = ypos;
			length = l;
		} // creates a new ceiling
}; // a ceiling on the stage
class Wall {
	public:
		int x, y; // x and y position of the wall
		int length; // how long the wall is
		string direction; // what direction it is facing
		Wall(int xpos, int ypos, int l, string d) {
			x = xpos;
			y = ypos;
			length = l;
			direction = d;
		} // creates a new wall
}; // a wall on the stage
class Stage {
	public:
	// variables
		int width, height; // width and height of the image of the stage
		int leftdeath, rightdeath, topdeath, bottomdeath;
		// where a person dies of the edges of the stage
		string name; // name of the stage
		vector<Floor> floors; // floors of the stage
		vector<Wall> walls; // walls of the stage
		vector<Ceiling> ceilings; // ceilings of the stage
		vector<Ledge> ledges; // ledges of the stage
		vector<int> spawnx; // where the x coords of the spawn points are
		vector<int> spawny; // where the y coords of the spawn points are
	// methods
		virtual void Initialize() {} 
		// each subclass has it's own initialization code
		string getName() { return name; }
		vector<Floor> getFloors() { return floors; }
		vector<Wall> getWalls() { return walls; }
		vector<Ceiling> getCeilings() { return ceilings; }
		vector<Ledge> getLedges() { return ledges; }
		// gets stuff about the stage
	//destructor
		virtual ~Stage() {}
}; // class which all stages extend
