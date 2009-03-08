#ifndef STAGE_H
#define STAGE_H
#include <vector>
#include <string>
using std::vector;
using std::string;

class Ledge {
	public:
		int x, y; // x and y position of the ledge
		string direction;
		Ledge(int xpos, int ypos, string dir);
}; // a ledge on the stage
class Floor {
	public:
		int x, y; // x and y of far left of stage
		int length; // total run of the floor
		bool isplatform; // is a platform of not
		vector<double> slopes; // sloping of the stage for each x position
		Floor(int xpos, int ypos, int l, bool plat);
		bool isPlatform(); // returns isplatform
		double totalrise(); // counts the total rise of the stage from start to finish
		double getrise(int distance); // the rise up to distance distance
}; // a floor on the stage
class Ceiling {
	public:
		int x, y; // x and y position of ceiling
		int length; // length of ceiling
		Ceiling(int xpos, int ypos, int l); // creates a new ceiling
}; // a ceiling on the stage
class Wall {
	public:
		int x, y; // x and y position of the wall
		int length; // how long the wall is
		string direction; // what direction it is facing
		Wall(int xpos, int ypos, int l, string d); // creates a new wall
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
		virtual void Initialize();
		// each subclass has it's own initialization code
		string getName();
		vector<Floor> getFloors();
		vector<Wall> getWalls();
		vector<Ceiling> getCeilings();
		vector<Ledge> getLedges();
		// gets stuff about the stage
	//destructor
		virtual ~Stage();
}; // class which all stages extend
#endif
